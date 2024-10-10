// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/xori-01.h"

namespace
{
class xori_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::xori_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::xori_01::generateCode().getCode(), scalarInitData);
        
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

std::vector<uint8_t> xori_01::s_ScalarData;
}


// Generate google tests

TEST_F(xori_01, opcode__xori___op1_x24__dest_x10__op1val_0x33333334___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[4], 3435973428);
}


TEST_F(xori_01, opcode__xori___op1_x18__dest_x18__op1val_0x4___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[5], 4);
}


TEST_F(xori_01, opcode__xori___op1_x15__dest_x24__op1val__0x4___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[6], 4294965251);
}


TEST_F(xori_01, opcode__xori___op1_x11__dest_x20__op1val_0x2___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[7], 3);
}


TEST_F(xori_01, opcode__xori___op1_x7__dest_x21__op1val__0x80000000___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[8], 2147485012);
}


TEST_F(xori_01, opcode__xori___op1_x17__dest_x27__op1val_0x0___immval__0x401) {
  ASSERT_EQ(getScalarWordData()[9], 4294966271);
}


TEST_F(xori_01, opcode__xori___op1_x22__dest_x1__op1val_0x7fffffff___immval__0xa) {
  ASSERT_EQ(getScalarWordData()[10], 2147483657);
}


TEST_F(xori_01, opcode__xori___op1_x20__dest_x22__op1val_0x1___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[11], 5);
}


TEST_F(xori_01, opcode__xori___op1_x19__dest_x31__op1val__0x201___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(xori_01, opcode__xori___op1_x9__dest_x5__op1val__0x21___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[13], 4294967261);
}


TEST_F(xori_01, opcode__xori___op1_x28__dest_x29__op1val__0x80000000___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[14], 2147483656);
}


TEST_F(xori_01, opcode__xori___op1_x30__dest_x4__op1val__0x40000001___immval_0x10) {
  ASSERT_EQ(getScalarWordData()[15], 3221225455);
}


TEST_F(xori_01, opcode__xori___op1_x27__dest_x8__op1val_0x7fffffff___immval_0x20) {
  ASSERT_EQ(getScalarWordData()[16], 2147483615);
}


TEST_F(xori_01, opcode__xori___op1_x3__dest_x25__op1val_0x66666666___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[17], 1717986854);
}


TEST_F(xori_01, opcode__xori___op1_x31__dest_x17__op1val__0x80001___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[18], 4294442879);
}


TEST_F(xori_01, opcode__xori___op1_x29__dest_x16__op1val__0xb504___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[19], 4294921212);
}


TEST_F(xori_01, opcode__xori___op1_x4__dest_x6__op1val_0x0___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[20], 512);
}


TEST_F(xori_01, opcode__xori___op1_x14__dest_x3__op1val__0x100001___immval_0x400) {
  ASSERT_EQ(getScalarWordData()[21], 4293917695);
}


TEST_F(xori_01, opcode__xori___op1_x12__dest_x15__op1val__0x7___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[22], 7);
}


TEST_F(xori_01, opcode__xori___op1_x21__dest_x9__op1val_0x5___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[23], 4294967288);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x19__op1val__0xb504___immval__0x5) {
  ASSERT_EQ(getScalarWordData()[24], 46343);
}


TEST_F(xori_01, opcode__xori___op1_x16__dest_x26__op1val_0x1___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[25], 4294967286);
}


TEST_F(xori_01, opcode__xori___op1_x26__dest_x30__op1val_0x66666667___immval__0x11) {
  ASSERT_EQ(getScalarWordData()[26], 2576980360);
}


TEST_F(xori_01, opcode__xori___op1_x1__dest_x23__op1val__0x41___immval__0x21) {
  ASSERT_EQ(getScalarWordData()[27], 96);
}


TEST_F(xori_01, opcode__xori___op1_x6__dest_x0__op1val__0x1___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(xori_01, opcode__xori___op1_x2__dest_x7__op1val_0x55555555___immval__0x81) {
  ASSERT_EQ(getScalarWordData()[29], 2863311402);
}


TEST_F(xori_01, opcode__xori___op1_x0__dest_x14__op1val_0x0___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[30], 4294967039);
}


TEST_F(xori_01, opcode__xori___op1_x8__dest_x13__op1val_0x3fffffff___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[31], 1073740458);
}


TEST_F(xori_01, opcode__xori___op1_x25__dest_x12__op1val__0x7___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[32], 1363);
}


TEST_F(xori_01, opcode__xori___op1_x13__dest_x11__op1val_0x8___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[33], 8);
}


TEST_F(xori_01, opcode__xori___op1_x23__dest_x2__op1val_0x10___immval_0x1) {
  ASSERT_EQ(getScalarWordData()[34], 17);
}


TEST_F(xori_01, opcode__xori___op1_x5__dest_x28__op1val_0x20___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[35], 1397);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x40___immval_0x100) {
  ASSERT_EQ(getScalarWordData()[36], 320);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x80___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[37], 133);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x100___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[38], 4294966975);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x200___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[39], 514);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x400___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[40], 4294966955);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x800___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[41], 2052);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x1000___immval_0x8) {
  ASSERT_EQ(getScalarWordData()[42], 4104);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2000___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[43], 9556);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4000___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[44], 16388);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x8000___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[45], 34815);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x10000___immval_0x40) {
  ASSERT_EQ(getScalarWordData()[46], 65600);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x20000___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[47], 131116);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x40000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[48], 4294703786);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x80000___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[49], 525107);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x100000___immval__0x6) {
  ASSERT_EQ(getScalarWordData()[50], 4293918714);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x200000___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[51], 4292870141);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x400000___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[52], 4195669);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x800000___immval__0x9) {
  ASSERT_EQ(getScalarWordData()[53], 4286578679);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x1000000___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[54], 16777221);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2000000___immval__0x201) {
  ASSERT_EQ(getScalarWordData()[55], 4261412351);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4000000___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[56], 67108864);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x8000000___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[57], 134218548);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x10000000___immval__0x41) {
  ASSERT_EQ(getScalarWordData()[58], 4026531775);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x20000000___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[59], 536872277);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x40000000___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[60], 3221224106);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[61], 4294967290);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x3___immval__0x3) {
  ASSERT_EQ(getScalarWordData()[62], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x5___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[63], 1019);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x9___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[64], 1372);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x11___immval_0x80) {
  ASSERT_EQ(getScalarWordData()[65], 4294967151);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x81___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[66], 4294965529);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x101___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[67], 4294967033);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x401___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[68], 4294966265);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x801___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[69], 4294965245);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x1001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[70], 4294963199);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x2001___immval_0x7ff) {
  ASSERT_EQ(getScalarWordData()[71], 4294957056);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x4001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[72], 16427);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x8001___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[73], 4294932890);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x10001___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[74], 4294900941);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x20001___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[75], 131328);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x40001___immval__0x800) {
  ASSERT_EQ(getScalarWordData()[76], 264191);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x200001___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[77], 4292870143);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x400001___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[78], 4290771354);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x800001___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[79], 4286577048);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x1000001___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[80], 16778581);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x2000001___immval__0x8) {
  ASSERT_EQ(getScalarWordData()[81], 33554439);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x4000001___immval__0x2) {
  ASSERT_EQ(getScalarWordData()[82], 67108865);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x8000001___immval__0x400) {
  ASSERT_EQ(getScalarWordData()[83], 134218751);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x10000001___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[84], 268435499);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x20000001___immval__0x101) {
  ASSERT_EQ(getScalarWordData()[85], 536871168);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x200) {
  ASSERT_EQ(getScalarWordData()[86], 2863311018);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[87], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[88], 1366);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[89], 4294965929);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[90], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[91], 816);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[92], 1637);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[93], 4294967248);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[94], 46);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[95], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[96], 1367);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[97], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[98], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[99], 817);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[100], 1638);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[101], 47);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[102], 1365);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[103], 4294965928);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[104], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[105], 823);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[106], 1636);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[107], 4294967255);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x3___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[108], 45);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[109], 1431655766);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[110], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[111], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[112], 1431655760);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[113], 1431656038);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[114], 1431655219);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[115], 2863311494);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[116], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[117], 1431655767);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[118], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[119], 1431655765);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[120], 1431655761);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[121], 1431656039);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[122], 1431655216);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[123], 1431655801);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[124], 1431654403);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[125], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[126], 1431655763);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[127], 1431656033);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[128], 1431655218);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[129], 2863311489);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[130], 1431655803);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[131], 2863311529);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[132], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[133], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[134], 2863311535);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[135], 2863311257);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[136], 2863312076);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[137], 1431655801);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[138], 2863311495);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[139], 2863311528);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[140], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[141], 2863311530);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[142], 2863311534);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[143], 2863311256);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[144], 2863312079);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[145], 2863311494);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[146], 2863312892);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[147], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[148], 2863311532);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[149], 2863311262);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[150], 2863312077);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[151], 1431655806);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[152], 2863311492);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[153], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[154], 1360);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[155], 4294965935);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[156], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[157], 822);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[158], 1635);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[159], 4294967254);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[160], 40);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[161], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[162], 1361);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[163], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[164], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[165], 823);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[166], 1632);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[167], 41);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[168], 1363);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[169], 4294965934);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[170], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[171], 817);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[172], 1634);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[173], 4294967249);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x5___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[174], 43);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[175], 858993456);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[176], 858994278);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[177], 3435973017);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[178], 858993462);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[179], 858992640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[180], 858994005);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[181], 3435973856);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[182], 858993438);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[183], 858993457);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[184], 858994279);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[185], 858993459);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[186], 858993463);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[187], 858992641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[188], 858994006);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[189], 858993439);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[190], 858994277);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[191], 3435973016);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[192], 858993461);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[193], 858992647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[194], 858994004);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[195], 3435973863);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333333___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[196], 858993437);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[197], 1717986917);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[198], 1717986099);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[199], 2576981196);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[200], 1717986915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[201], 1717986645);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[202], 1717985280);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[203], 2576980405);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[204], 1717986891);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[205], 1717986916);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[206], 1717986098);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[207], 1717986918);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[208], 1717986914);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[209], 1717986644);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[210], 1717985283);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[211], 1717986890);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[212], 1717986096);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[213], 2576981197);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[214], 1717986912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[215], 1717986642);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[216], 1717985281);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[217], 2576980402);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666666___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[218], 1717986888);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[219], 4294920959);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[220], 4294922153);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[221], 45142);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[222], 4294920953);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[223], 4294920655);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[224], 4294921370);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[225], 46383);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[226], 4294920913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[227], 4294920958);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[228], 4294922152);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[229], 4294920956);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[230], 4294920952);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[231], 4294920654);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[232], 4294921369);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[233], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[234], 4294922154);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[235], 45143);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[236], 4294920954);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[237], 4294920648);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[238], 4294921371);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[239], 46376);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[240], 4294920914);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[241], 46343);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[242], 45137);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[243], 4294922158);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[244], 46337);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[245], 46647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[246], 45922);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[247], 4294920919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[248], 46377);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[249], 46342);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[250], 45136);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[251], 46340);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[252], 46336);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[253], 46646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[254], 45921);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[255], 46376);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[256], 45138);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[257], 4294922159);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[258], 46338);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[259], 46640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[260], 45923);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[261], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb504___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[262], 46378);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[263], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[264], 1367);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[265], 4294965928);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[266], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[267], 817);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[268], 1636);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[269], 4294967249);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[270], 47);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[271], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[272], 1366);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[273], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[274], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[275], 816);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[276], 1639);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[277], 46);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[278], 1364);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[279], 4294965929);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[280], 4);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[281], 822);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[282], 1637);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[283], 4294967254);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x2___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[284], 44);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[285], 1431655767);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[286], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[287], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[288], 1431655761);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[289], 1431656039);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[290], 1431655218);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[291], 2863311495);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[292], 1431655801);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[293], 1431655766);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[294], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[295], 1431655764);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[296], 1431655760);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[297], 1431656038);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[298], 1431655217);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[299], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[300], 1431654402);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[301], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[302], 1431655762);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[303], 1431656032);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[304], 1431655219);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[305], 2863311488);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555554___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[306], 1431655802);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[307], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[308], 1365);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[309], 4294965930);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[310], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[311], 819);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[312], 1638);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[313], 4294967251);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[314], 45);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[315], 858992646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[316], 858994001);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[317], 858993432);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[318], 858994274);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[319], 3435973023);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[320], 858993458);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[321], 858992640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[322], 858994003);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[323], 3435973856);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[324], 858993434);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[325], 1717986916);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[326], 1717986098);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[327], 2576981197);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[328], 1717986914);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[329], 1717986644);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[330], 1717985281);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[331], 2576980404);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[332], 1717986890);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[333], 1717986917);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[334], 1717986099);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[335], 1717986919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[336], 1717986915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[337], 1717986645);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[338], 1717985282);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[339], 1717986891);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[340], 1717986097);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[341], 2576981196);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[342], 1717986913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[343], 1717986643);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[344], 1717985280);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[345], 2576980403);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666667___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[346], 1717986889);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[347], 4294920958);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[348], 4294922152);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[349], 45143);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[350], 4294920952);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[351], 4294920654);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[352], 4294921371);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[353], 46382);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[354], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[355], 4294920959);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[356], 4294922153);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[357], 4294920957);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[358], 4294920953);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[359], 4294920655);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[360], 4294921368);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[361], 4294920913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[362], 4294922155);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[363], 45142);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[364], 4294920955);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[365], 4294920649);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[366], 4294921370);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[367], 46377);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[368], 4294920915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[369], 46342);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[370], 45136);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[371], 4294922159);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[372], 46336);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[373], 46646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[374], 45923);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[375], 4294920918);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[376], 46376);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[377], 46343);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[378], 45137);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[379], 46341);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[380], 46337);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[381], 46647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[382], 45920);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[383], 46377);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[384], 45139);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[385], 4294922158);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[386], 46339);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[387], 46641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[388], 45922);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[389], 4294920913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb505___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[390], 46379);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[391], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[392], 1364);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[393], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[394], 4);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[395], 818);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[396], 1637);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[397], 44);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[398], 1366);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[399], 4294965931);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[400], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[401], 820);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[402], 1639);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[403], 4294967252);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x0___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[404], 46);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[405], 7);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[406], 1361);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[407], 4294965934);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[408], 1);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[409], 823);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[410], 1634);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[411], 4294967255);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[412], 41);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[413], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[414], 1360);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[415], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[416], 822);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[417], 1633);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[418], 40);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[419], 1362);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[420], 4294965935);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[421], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[422], 816);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[423], 1635);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[424], 4294967248);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x4___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[425], 42);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[426], 858993457);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[427], 858994279);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[428], 3435973016);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[429], 858993463);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[430], 858992641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[431], 858994004);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[432], 3435973857);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[433], 858993439);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[434], 858993456);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[435], 858994278);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[436], 858993458);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[437], 858993462);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[438], 858992640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[439], 858994007);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[440], 858993438);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[441], 858994276);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[442], 3435973017);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[443], 858993460);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[444], 858992646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[445], 858994005);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[446], 3435973862);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333332___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[447], 858993436);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[448], 1717986918);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[449], 1717986096);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[450], 2576981199);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[451], 1717986912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[452], 1717986646);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[453], 1717985283);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[454], 2576980406);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[455], 1717986888);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[456], 1717986919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[457], 1717986097);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[458], 1717986917);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[459], 1717986913);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[460], 1717986647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[461], 1717985280);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[462], 1717986889);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[463], 1717986099);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[464], 2576981198);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[465], 1717986915);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[466], 1717986641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[467], 1717985282);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[468], 2576980401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x66666665___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[469], 1717986891);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[470], 46336);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[471], 45142);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[472], 4294922153);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[473], 46342);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[474], 46640);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[475], 45925);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[476], 4294920912);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[477], 46382);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[478], 46337);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[479], 45143);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[480], 46339);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[481], 46343);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[482], 46641);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[483], 45926);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[484], 46383);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[485], 45141);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[486], 4294922152);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[487], 46341);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[488], 46647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[489], 45924);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[490], 4294920919);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0xb503___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[491], 46381);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[492], 1431655765);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[493], 1431654403);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[494], 2863312892);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[495], 1431655763);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[496], 1431656037);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[497], 1431655216);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[498], 2863311493);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[499], 1431655803);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[500], 1431655764);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[501], 1431654402);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[502], 1431655766);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[503], 1431655762);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[504], 1431656036);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[505], 1431655219);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[506], 1431655802);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[507], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[508], 2863312893);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[509], 1431655760);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[510], 1431656034);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[511], 1431655217);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[512], 2863311490);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x55555556___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[513], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[514], 2863311528);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[515], 2863312894);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[516], 1431654401);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[517], 2863311534);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[518], 2863311256);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[519], 2863312077);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[520], 1431655800);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[521], 2863311494);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[522], 2863311529);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[523], 2863312895);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[524], 2863311531);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[525], 2863311535);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[526], 2863311257);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[527], 2863312078);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[528], 2863311495);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[529], 2863312893);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[530], 1431654400);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[531], 2863311533);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[532], 2863311263);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[533], 2863312076);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[534], 1431655807);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val__0x55555555___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[535], 2863311493);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[536], 5);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[537], 1363);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[538], 4294965932);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[539], 3);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[540], 821);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[541], 1632);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[542], 4294967253);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[543], 43);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[544], 4);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[545], 1362);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[546], 6);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[547], 2);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x332) {
  ASSERT_EQ(getScalarWordData()[548], 820);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x665) {
  ASSERT_EQ(getScalarWordData()[549], 1635);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2c) {
  ASSERT_EQ(getScalarWordData()[550], 42);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x556) {
  ASSERT_EQ(getScalarWordData()[551], 1360);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x555) {
  ASSERT_EQ(getScalarWordData()[552], 4294965933);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x6) {
  ASSERT_EQ(getScalarWordData()[553], 0);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x334) {
  ASSERT_EQ(getScalarWordData()[554], 818);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x667) {
  ASSERT_EQ(getScalarWordData()[555], 1633);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval__0x2c) {
  ASSERT_EQ(getScalarWordData()[556], 4294967250);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x6___immval_0x2e) {
  ASSERT_EQ(getScalarWordData()[557], 40);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x3) {
  ASSERT_EQ(getScalarWordData()[558], 858993463);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x555) {
  ASSERT_EQ(getScalarWordData()[559], 858994273);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x556) {
  ASSERT_EQ(getScalarWordData()[560], 3435973022);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x5) {
  ASSERT_EQ(getScalarWordData()[561], 858993457);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x333) {
  ASSERT_EQ(getScalarWordData()[562], 858992647);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x666) {
  ASSERT_EQ(getScalarWordData()[563], 858994002);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval__0x2d) {
  ASSERT_EQ(getScalarWordData()[564], 3435973863);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2d) {
  ASSERT_EQ(getScalarWordData()[565], 858993433);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x2) {
  ASSERT_EQ(getScalarWordData()[566], 858993462);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x554) {
  ASSERT_EQ(getScalarWordData()[567], 858994272);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x0) {
  ASSERT_EQ(getScalarWordData()[568], 858993460);
}


TEST_F(xori_01, opcode__xori___op1_x10__dest_x11__op1val_0x33333334___immval_0x4) {
  ASSERT_EQ(getScalarWordData()[569], 858993456);
}

