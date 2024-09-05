// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/addi-01.h"

namespace
{
class addi_01Fixture : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = addi_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(addi_01::generateCode().getCode(), scalarInitData);
        
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

std::vector<uint8_t> addi_01Fixture::s_ScalarData;
}


// Generate google tests

TEST_F(addi_01Fixture, opcode__addi___op1_x20__dest_x7__op1val_0x20000000___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[4], 536868864);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x3__dest_x3__op1val_0x400___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[5], 1024);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x4__dest_x22__op1val__0x201___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[6], 1534);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x30__dest_x11__op1val_0x0___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[7], 1);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x27__dest_x31__op1val__0x80000000___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[8], 2147483664);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x17__dest_x30__op1val_0x7fffffff___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[9], 2147483653);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x18__dest_x28__op1val_0x1___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[10], 5);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x13__dest_x6__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[11], 10);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x16__op1val__0x55555555___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[12], 2863311498);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x9__dest_x21__op1val__0x11___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[13], 4294967281);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x7__dest_x2__op1val_0xb505___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[14], 46349);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x22__dest_x18__op1val__0xb504___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[15], 4294920988);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x29__dest_x0__op1val__0x200001___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x25__dest_x13__op1val_0x5___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[17], 133);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x11__dest_x29__op1val__0x2000001___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[18], 4261413119);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x6__dest_x8__op1val_0x10___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[19], 528);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x19__dest_x4__op1val_0x2___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[20], 1026);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x12__dest_x10__op1val_0x55555554___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[21], 1431655762);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x31__dest_x26__op1val_0x55555555___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[22], 1431655762);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x26__dest_x15__op1val_0x55555554___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[23], 1431655759);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x5__dest_x19__op1val_0x3___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[24], 4294967290);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x0__dest_x1__op1val_0x0___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[25], 4294967279);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x24__dest_x23__op1val_0x800000___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[26], 8388543);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x15__dest_x27__op1val__0x11___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[27], 4294967150);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x14__dest_x17__op1val_0x55555555___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[28], 1431655508);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x21__dest_x25__op1val_0x55555556___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[29], 1431655253);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x1__dest_x24__op1val_0x2000___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[30], 7167);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x16__dest_x12__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[31], 2863312895);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x2__dest_x14__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[32], 4294965930);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x23__dest_x20__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[33], 8);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x28__dest_x5__op1val_0x8___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[34], 24);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x8__dest_x9__op1val_0x20___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[35], 4294967284);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x40___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[36], 4294967295);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x80___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[37], 133);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x100___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[38], 4294966528);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x200___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[39], 768);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x800___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[40], 2038);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x1000___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[41], 4160);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4000___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[42], 17202);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x8000___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[43], 31744);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x10000___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[44], 65279);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x20000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[45], 131076);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x40000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[46], 262144);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x80000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[47], 524293);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x100000___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[48], 1048620);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x200000___immval__0x4) {
  ASSERT_EQ(getScalarWordData()[49], 2097148);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x400000___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[50], 4194350);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x1000000___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[51], 16776192);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2000000___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[52], 33554415);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4000000___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[53], 67108862);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x8000000___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[54], 134217731);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x10000000___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[55], 268435327);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x40000000___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[56], 1073741822);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x2___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[57], 4294966781);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x3___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[58], 4294967284);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[59], 4294967246);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x9___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[60], 811);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x21___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[61], 4294967258);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x41___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[62], 4294967222);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x81___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[63], 4294967150);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x101___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[64], 255);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x401___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[65], 4294966227);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x801___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[66], 4294965251);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x1001___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[67], 4294963207);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x2001___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[68], 4294958590);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x4001___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[69], 4294951935);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x8001___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[70], 4294933161);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x10001___immval_0x7) {
  ASSERT_EQ(getScalarWordData()[71], 4294901766);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x20001___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[72], 4294836351);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x40001___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[73], 4294705159);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x80001___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[74], 4294440959);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x100001___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[75], 4293918686);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x400001___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[76], 4290774355);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x800001___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[77], 4286578677);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x1000001___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[78], 4278190123);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x4000001___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[79], 4227858437);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x8000001___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[80], 4160751205);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x10000001___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[81], 4026531885);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x20000001___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[82], 3758096386);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x40000001___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[83], 3221227109);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[84], 6);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[85], 1368);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[86], 4294965933);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[87], 8);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[88], 822);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[89], 1641);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[90], 4294967254);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[91], 48);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[92], 5);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[93], 1367);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[94], 3);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[95], 7);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[96], 821);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[97], 1640);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[98], 47);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[99], 1369);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[100], 4294965934);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[101], 9);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[102], 823);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[103], 1642);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[104], 4294967255);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[105], 49);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[106], 1431655768);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[107], 1431657130);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[108], 1431654399);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[109], 1431655770);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[110], 1431656584);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[111], 1431657403);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[112], 1431655720);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[113], 1431655810);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[114], 1431655767);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[115], 1431657129);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[116], 1431655765);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[117], 1431655769);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[118], 1431656583);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[119], 1431657402);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[120], 1431655809);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[121], 1431657131);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[122], 1431654400);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[123], 1431655771);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[124], 1431656585);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[125], 1431657404);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[126], 1431655721);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[127], 1431655811);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[128], 2863311533);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[129], 2863310164);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[130], 2863311535);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[131], 2863312349);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[132], 2863313168);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[133], 2863311485);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[134], 2863311575);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[135], 2863311532);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[136], 2863312894);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[137], 2863311530);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[138], 2863311534);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[139], 2863312348);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[140], 2863313167);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[141], 2863311574);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[142], 2863312896);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[143], 2863310165);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[144], 2863311536);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[145], 2863312350);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[146], 2863313169);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[147], 2863311486);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[148], 2863311576);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[149], 8);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[150], 1370);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[151], 4294965935);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[152], 824);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[153], 1643);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[154], 4294967256);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[155], 50);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[156], 7);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[157], 1369);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[158], 5);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[159], 9);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[160], 823);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[161], 1642);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[162], 49);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[163], 1371);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[164], 4294965936);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[165], 11);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[166], 825);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[167], 1644);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[168], 4294967257);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[169], 51);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[170], 858993462);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[171], 858994824);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[172], 858992093);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[173], 858993464);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[174], 858994278);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[175], 858995097);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[176], 858993414);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[177], 858993504);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[178], 858993461);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[179], 858994823);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[180], 858993459);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[181], 858993463);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[182], 858994277);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[183], 858995096);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[184], 858993503);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[185], 858994825);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[186], 858992094);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[187], 858993465);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[188], 858994279);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[189], 858995098);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[190], 858993415);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[191], 858993505);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[192], 1717986921);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[193], 1717988283);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[194], 1717985552);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[195], 1717986923);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[196], 1717987737);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[197], 1717988556);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[198], 1717986873);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[199], 1717986963);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[200], 1717986920);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[201], 1717988282);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[202], 1717986918);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[203], 1717986922);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[204], 1717987736);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[205], 1717988555);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[206], 1717986962);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[207], 1717988284);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[208], 1717985553);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[209], 1717986924);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[210], 1717987738);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[211], 1717988557);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[212], 1717986874);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[213], 1717986964);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[214], 4294920959);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[215], 4294922321);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[216], 4294919590);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[217], 4294920961);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[218], 4294921775);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[219], 4294922594);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[220], 4294920911);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[221], 4294921001);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[222], 4294920958);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[223], 4294922320);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[224], 4294920956);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[225], 4294920960);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[226], 4294921774);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[227], 4294922593);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[228], 4294921000);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[229], 4294922322);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[230], 4294919591);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[231], 4294920962);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[232], 4294921776);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[233], 4294922595);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[234], 4294920912);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[235], 4294921002);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[236], 46343);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[237], 47705);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[238], 44974);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[239], 46345);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[240], 47159);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[241], 47978);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[242], 46295);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[243], 46385);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[244], 46342);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[245], 47704);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[246], 46340);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[247], 46344);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[248], 47158);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[249], 47977);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[250], 46384);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[251], 47706);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[252], 44975);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[253], 46346);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[254], 47160);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[255], 47979);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[256], 46296);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[257], 46386);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[258], 5);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[259], 1367);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[260], 4294965932);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[261], 7);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[262], 821);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[263], 1640);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[264], 4294967253);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[265], 47);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[266], 4);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[267], 1366);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[268], 2);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[269], 6);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[270], 820);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[271], 1639);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[272], 46);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[273], 1368);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[274], 4294965933);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[275], 8);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[276], 822);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[277], 1641);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[278], 4294967254);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[279], 48);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[280], 1431655767);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[281], 1431657129);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[282], 1431654398);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[283], 1431655769);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[284], 1431656583);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[285], 1431657402);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[286], 1431655719);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[287], 1431655809);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[288], 1431655766);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[289], 1431657128);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[290], 1431655764);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[291], 1431655768);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[292], 1431656582);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[293], 1431657401);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[294], 1431655808);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[295], 1431657130);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[296], 1431654399);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[297], 1431655770);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[298], 1431656584);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[299], 1431657403);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[300], 1431655720);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[301], 1431655810);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[302], 3);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[303], 1365);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[304], 5);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[305], 819);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[306], 1638);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[307], 4294967251);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[308], 45);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[309], 858994278);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[310], 858995097);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[311], 858993504);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[312], 858994826);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[313], 858992095);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[314], 858993466);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[315], 858994280);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[316], 858995099);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[317], 858993416);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[318], 858993506);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[319], 1717986922);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[320], 1717988284);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[321], 1717985553);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[322], 1717986924);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[323], 1717987738);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[324], 1717988557);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[325], 1717986874);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[326], 1717986964);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[327], 1717986921);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[328], 1717988283);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[329], 1717986919);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[330], 1717986923);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[331], 1717987737);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[332], 1717988556);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[333], 1717986963);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[334], 1717988285);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[335], 1717985554);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[336], 1717986925);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[337], 1717987739);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[338], 1717988558);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[339], 1717986875);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[340], 1717986965);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[341], 4294920960);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[342], 4294922322);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[343], 4294919591);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[344], 4294920962);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[345], 4294921776);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[346], 4294922595);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[347], 4294920912);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[348], 4294921002);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[349], 4294920959);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[350], 4294922321);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[351], 4294920957);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[352], 4294920961);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[353], 4294921775);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[354], 4294922594);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[355], 4294921001);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[356], 4294922323);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[357], 4294919592);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[358], 4294920963);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[359], 4294921777);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[360], 4294922596);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[361], 4294920913);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[362], 4294921003);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[363], 46344);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[364], 47706);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[365], 44975);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[366], 46346);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[367], 47160);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[368], 47979);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[369], 46296);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[370], 46386);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[371], 46343);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[372], 47705);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[373], 46341);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[374], 46345);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[375], 47159);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[376], 47978);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[377], 46385);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[378], 47707);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[379], 44976);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[380], 46347);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[381], 47161);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[382], 47980);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[383], 46297);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[384], 46387);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[385], 2);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[386], 1364);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[387], 0);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[388], 4);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[389], 818);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[390], 1637);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[391], 44);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[392], 1366);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[393], 4294965931);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[394], 6);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[395], 820);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[396], 1639);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[397], 4294967252);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[398], 46);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[399], 7);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[400], 1369);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[401], 4294965934);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[402], 9);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[403], 823);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[404], 1642);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[405], 4294967255);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[406], 49);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[407], 6);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[408], 1368);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[409], 4);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[410], 822);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[411], 1641);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[412], 48);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[413], 1370);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[414], 4294965935);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[415], 10);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[416], 824);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[417], 1643);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[418], 4294967256);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[419], 50);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[420], 858993461);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[421], 858994823);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[422], 858992092);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[423], 858993463);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[424], 858994277);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[425], 858995096);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[426], 858993413);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[427], 858993503);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[428], 858993460);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[429], 858994822);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[430], 858993458);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[431], 858993462);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[432], 858994276);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[433], 858995095);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[434], 858993502);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[435], 858994824);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[436], 858992093);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[437], 858993464);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[438], 858994278);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[439], 858995097);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[440], 858993414);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[441], 858993504);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[442], 1717986920);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[443], 1717988282);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[444], 1717985551);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[445], 1717986922);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[446], 1717987736);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[447], 1717988555);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[448], 1717986872);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[449], 1717986962);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[450], 1717986919);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[451], 1717988281);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[452], 1717986917);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[453], 1717986921);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[454], 1717987735);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[455], 1717988554);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[456], 1717986961);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[457], 1717988283);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[458], 1717985552);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[459], 1717986923);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[460], 1717987737);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[461], 1717988556);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[462], 1717986873);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[463], 1717986963);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[464], 46342);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[465], 47704);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[466], 44973);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[467], 46344);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[468], 47158);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[469], 47977);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[470], 46294);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[471], 46384);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[472], 46341);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[473], 47703);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[474], 46339);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[475], 46343);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[476], 47157);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[477], 47976);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[478], 46383);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[479], 47705);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[480], 44974);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[481], 46345);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[482], 47159);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[483], 47978);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[484], 46295);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[485], 46385);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[486], 1431655769);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[487], 1431657131);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[488], 1431654400);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[489], 1431655771);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[490], 1431656585);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[491], 1431657404);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[492], 1431655721);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[493], 1431655811);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[494], 1431655768);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[495], 1431657130);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[496], 1431655766);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[497], 1431655770);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[498], 1431656584);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[499], 1431657403);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[500], 1431655810);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[501], 1431657132);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[502], 1431654401);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[503], 1431655772);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[504], 1431656586);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[505], 1431657405);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[506], 1431655722);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[507], 1431655812);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[508], 2863311534);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[509], 2863312896);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[510], 2863310165);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[511], 2863311536);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[512], 2863312350);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[513], 2863313169);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[514], 2863311486);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[515], 2863311576);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[516], 2863311533);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[517], 2863312895);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[518], 2863311531);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[519], 2863311535);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[520], 2863312349);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[521], 2863313168);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[522], 2863311575);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[523], 2863312897);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[524], 2863310166);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[525], 2863311537);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[526], 2863312351);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[527], 2863313170);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[528], 2863311487);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[529], 2863311577);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[530], 9);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[531], 1371);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[532], 4294965936);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[533], 11);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[534], 825);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[535], 1644);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[536], 4294967257);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[537], 51);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[538], 8);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[539], 1370);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[540], 6);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[541], 10);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[542], 824);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[543], 1643);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[544], 50);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[545], 1372);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[546], 4294965937);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[547], 12);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[548], 826);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[549], 1645);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[550], 4294967258);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[551], 52);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[552], 858993463);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[553], 858994825);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[554], 858992094);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[555], 858993465);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[556], 858994279);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[557], 858995098);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[558], 858993415);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[559], 858993505);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[560], 858993462);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[561], 858994824);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[562], 858993460);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[563], 858993464);
}


TEST_F(addi_01Fixture, opcode__addi___op1_x10__dest_x11__op1val__0x200001___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[564], 4292870207);
}

