// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/sub-01.h"

namespace
{
class sub_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::sub_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(ArchTest::sub_01::generateCode(true), scalarInitData);
        
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

std::vector<uint8_t> sub_01::s_ScalarData;
}


// Generate google tests

TEST_F(sub_01, opcode__sub___op1_x24__op2_x26__dest_x26__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[5], 1431655758);
}


TEST_F(sub_01, opcode__sub___op1_x17__op2_x17__dest_x23__op1val_0x2000000___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[6], 0);
}


TEST_F(sub_01, opcode__sub___op1_x16__op2_x16__dest_x16__op1val__0x7___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[7], 0);
}


TEST_F(sub_01, opcode__sub___op1_x31__op2_x19__dest_x31__op1val__0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[8], 2576980376);
}


TEST_F(sub_01, opcode__sub___op1_x23__op2_x14__dest_x8__op1val_0x80000___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(sub_01, opcode__sub___op1_x13__op2_x24__dest_x18__op1val__0x4000001___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[10], 2080374783);
}


TEST_F(sub_01, opcode__sub___op1_x12__op2_x4__dest_x0__op1val_0x20___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[11], 0);
}


TEST_F(sub_01, opcode__sub___op1_x22__op2_x9__dest_x10__op1val__0x20000001___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[12], 1610612736);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x27__dest_x25__op1val_0x10000___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[13], 65535);
}


TEST_F(sub_01, opcode__sub___op1_x8__op2_x3__dest_x14__op1val__0x80000000___op2val__0x40000000) {
  ASSERT_EQ(getScalarWordData()[14], 3221225472);
}


TEST_F(sub_01, opcode__sub___op1_x25__op2_x30__dest_x29__op1val_0x0___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[15], 4292870144);
}


TEST_F(sub_01, opcode__sub___op1_x18__op2_x8__dest_x15__op1val_0x7fffffff___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[16], 2147475455);
}


TEST_F(sub_01, opcode__sub___op1_x14__op2_x15__dest_x3__op1val_0x1___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[17], 4294967281);
}


TEST_F(sub_01, opcode__sub___op1_x26__op2_x29__dest_x13__op1val__0x7___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[18], 4294967287);
}


TEST_F(sub_01, opcode__sub___op1_x21__op2_x31__dest_x19__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[19], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x30__op2_x5__dest_x11__op1val__0x1000001___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[20], 4278190071);
}


TEST_F(sub_01, opcode__sub___op1_x28__op2_x7__dest_x30__op1val__0x55555556___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[21], 2863311498);
}


TEST_F(sub_01, opcode__sub___op1_x9__op2_x10__dest_x7__op1val__0x41___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[22], 4294967167);
}


TEST_F(sub_01, opcode__sub___op1_x0__op2_x18__dest_x17__op1val_0x0___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[23], 4294967168);
}


TEST_F(sub_01, opcode__sub___op1_x2__op2_x12__dest_x27__op1val__0x40000000___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[24], 3221225216);
}


TEST_F(sub_01, opcode__sub___op1_x4__op2_x28__dest_x20__op1val__0xb503___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[25], 4294920445);
}


TEST_F(sub_01, opcode__sub___op1_x15__op2_x1__dest_x6__op1val_0x0___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[26], 4294966272);
}


TEST_F(sub_01, opcode__sub___op1_x5__op2_x0__dest_x9__op1val__0x80000000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[27], 2147483648);
}


TEST_F(sub_01, opcode__sub___op1_x7__op2_x11__dest_x4__op1val_0x20000___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[28], 126976);
}


TEST_F(sub_01, opcode__sub___op1_x19__op2_x6__dest_x1__op1val__0x10000001___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[29], 4026515455);
}


TEST_F(sub_01, opcode__sub___op1_x6__op2_x20__dest_x5__op1val_0x7fffffff___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[30], 2147450879);
}


TEST_F(sub_01, opcode__sub___op1_x29__op2_x22__dest_x2__op1val__0xb503___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[31], 4294855421);
}


TEST_F(sub_01, opcode__sub___op1_x3__op2_x13__dest_x21__op1val__0x800001___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[32], 4286447615);
}


TEST_F(sub_01, opcode__sub___op1_x1__op2_x21__dest_x28__op1val_0x100___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[33], 4294705408);
}


TEST_F(sub_01, opcode__sub___op1_x20__op2_x23__dest_x12__op1val_0x100___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[34], 4293918976);
}


TEST_F(sub_01, opcode__sub___op1_x27__op2_x25__dest_x22__op1val__0x4___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[35], 4290772988);
}


TEST_F(sub_01, opcode__sub___op1_x11__op2_x2__dest_x24__op1val_0x66666665___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[36], 1709598309);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3fffffff___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[37], 1056964607);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[38], 4261366524);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[39], 4227825663);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[40], 4160749572);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[41], 4026527743);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[42], 3758092287);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[43], 3221749760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[44], 1431655767);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[45], 4294901762);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[46], 261);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[47], 33554441);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[48], 268435473);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[49], 35);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[50], 2863311595);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[51], 46598);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[52], 46854);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3fffffff___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[53], 1073742848);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[54], 4294923005);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[55], 4064);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[56], 7168);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[57], 4294918144);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[58], 32771);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[59], 63488);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[60], 4286709760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x101___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[61], 261888);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[62], 4161273856);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[63], 4291821568);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[64], 2097150);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[65], 4164943872);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[66], 4169138176);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[67], 875770677);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[68], 1465210198);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[69], 67108868);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[70], 993211189);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[71], 268481796);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[72], 536870917);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[73], 1073741825);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[74], 2863311787);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[75], 1431917910);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[76], 4105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[77], 262161);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[78], 1431655830);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[79], 8321);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[80], 33281);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[81], 1021);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[82], 2099201);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[83], 4080);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[84], 4286586880);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[85], 4294852608);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val__0x1) {
  ASSERT_EQ(getScalarWordData()[86], 32769);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[87], 1048568);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[88], 2097169);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[89], 2867505835);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[90], 8388641);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[91], 16908289);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[92], 67108858);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[93], 134217720);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[94], 553648129);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[95], 1073741821);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[96], 3435973835);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[97], 4294967286);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[98], 4294967287);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[99], 131056);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[100], 4294966143);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[101], 4286578175);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[102], 4294959105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[103], 29955);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[104], 393216);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[105], 4294705135);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[106], 4294489347);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[107], 4293918703);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[108], 4292870150);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x2000001___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[109], 3402419405);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[110], 2362232012);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[111], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[112], 2863311534);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[113], 1431655769);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[114], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[115], 3435973840);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[116], 2576980381);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[117], 46343);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[118], 4294920959);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[119], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[120], 2863311535);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[121], 3);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[122], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[123], 3435973841);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[124], 2576980382);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[125], 4294920960);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[126], 2863311533);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[127], 1431655768);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[128], 4294967293);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[129], 3435973839);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[130], 2576980380);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[131], 46342);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[132], 4294920958);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[133], 1431655762);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[134], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[135], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[136], 1431655760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[137], 572662306);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[138], 4008636143);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[139], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[140], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[141], 1431655763);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[142], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[143], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[144], 1431655761);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[145], 572662307);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[146], 4008636144);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[147], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[148], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[149], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[150], 1431655759);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[151], 572662305);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[152], 4008636142);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[153], 1431702104);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[154], 1431609424);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[155], 2863311527);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[156], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[157], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[158], 2863311525);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[159], 2004318071);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[160], 1145324612);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[161], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[162], 2863265190);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[163], 2863311528);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[164], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[165], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[166], 2863311526);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[167], 2004318072);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[168], 1145324613);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[169], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[170], 1431655764);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[171], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[172], 2863311524);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[173], 2004318070);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[174], 1145324611);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[175], 2863357869);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[176], 2863265189);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[177], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[178], 2863311536);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[179], 1431655771);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[180], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[181], 3435973842);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[182], 2576980383);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[183], 46345);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[184], 4294920961);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[185], 3);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[186], 2863311537);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[187], 5);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[188], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[189], 3435973843);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[190], 2576980384);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[191], 4294920962);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[192], 2863311535);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[193], 1431655770);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[194], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[195], 3435973841);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[196], 2576980382);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[197], 46344);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[198], 4294920960);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[199], 858993456);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[200], 3722304990);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[201], 2290649225);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[202], 858993454);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[203], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[204], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[205], 859039799);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[206], 858947119);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[207], 858993457);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[208], 3722304991);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[209], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[210], 858993455);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[211], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[212], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[213], 858947120);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[214], 3722304989);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[215], 2290649224);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[216], 858993453);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[217], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[218], 3435973836);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[219], 859039798);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[220], 858947118);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[221], 1717986915);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[222], 286331153);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[223], 3149642684);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[224], 1717986913);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[225], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[226], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[227], 1718033258);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[228], 1717940578);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[229], 1717986916);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[230], 286331154);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[231], 1717986918);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[232], 1717986914);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[233], 858993460);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[234], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[235], 1717940579);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[236], 286331152);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[237], 3149642683);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[238], 1717986912);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[239], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[240], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[241], 1718033257);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[242], 1717940577);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[243], 4294920953);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[244], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[245], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[246], 4294920951);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[247], 3435927497);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[248], 2576934038);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[249], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[250], 4294874616);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[251], 4294920954);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[252], 2863265192);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[253], 4294920956);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[254], 4294920952);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[255], 3435927498);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[256], 2576934039);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[257], 4294874617);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[258], 2863265190);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[259], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[260], 4294920950);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[261], 3435927496);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[262], 2576934037);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[263], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[264], 4294874615);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[265], 46337);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[266], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[267], 1431702106);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[268], 46335);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[269], 3436020177);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[270], 2577026718);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[271], 92680);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[272], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[273], 46338);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[274], 2863357872);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[275], 46340);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[276], 46336);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[277], 3436020178);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[278], 2577026719);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[279], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[280], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[281], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[282], 46334);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[283], 3436020176);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[284], 2577026717);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[285], 92679);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[286], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[287], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[288], 2863311533);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[289], 1431655768);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[290], 4294967293);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[291], 3435973839);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[292], 2576980380);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[293], 46342);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[294], 4294920958);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[295], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[296], 2863311534);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[297], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[298], 3435973840);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[299], 2576980381);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[300], 4294920959);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[301], 2863311532);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[302], 1431655767);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[303], 4294967292);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[304], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[305], 2576980379);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[306], 46341);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[307], 4294920957);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[308], 1431655761);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[309], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[310], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[311], 1431655759);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[312], 572662305);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[313], 4008636142);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[314], 1431702104);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[315], 1431609424);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[316], 1431655762);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[317], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[318], 1431655764);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[319], 1431655760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[320], 572662306);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[321], 4008636143);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[322], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[323], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[324], 2863311529);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[325], 572662304);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[326], 4008636141);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[327], 1431702103);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[328], 1431609423);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[329], 4294967293);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[330], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[331], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[332], 4294967291);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[333], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[334], 2576980378);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[335], 46340);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[336], 4294920956);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[337], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[338], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[339], 2863311525);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[340], 2004318071);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[341], 1145324612);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[342], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[343], 2863265190);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[344], 3);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[345], 2863311537);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[346], 1431655772);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[347], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[348], 3435973843);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[349], 2576980384);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[350], 46346);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[351], 4294920962);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[352], 4);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[353], 2863311538);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[354], 6);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[355], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[356], 3435973844);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[357], 2576980385);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[358], 4294920963);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[359], 2863311536);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[360], 1431655771);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[361], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[362], 3435973842);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[363], 2576980383);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[364], 46345);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[365], 4294920961);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[366], 858993457);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[367], 3722304991);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[368], 2290649226);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[369], 858993455);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[370], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[371], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[372], 859039800);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[373], 858947120);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[374], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[375], 3722304992);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[376], 858993460);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[377], 858993456);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[378], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[379], 3435973839);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[380], 858947121);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[381], 3722304990);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[382], 2290649225);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[383], 858993454);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[384], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[385], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[386], 859039799);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[387], 858947119);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[388], 1717986916);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[389], 286331154);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[390], 3149642685);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[391], 1717986914);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[392], 858993460);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[393], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[394], 1718033259);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[395], 1717940579);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[396], 1717986917);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[397], 286331155);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[398], 1717986919);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[399], 1717986915);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[400], 858993461);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[401], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[402], 1717940580);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[403], 286331153);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[404], 3149642684);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[405], 1717986913);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[406], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[407], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[408], 1718033258);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[409], 1717940578);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[410], 4294920954);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[411], 2863265192);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[412], 1431609427);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[413], 4294920952);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[414], 3435927498);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[415], 2576934039);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[416], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[417], 4294874617);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[418], 4294920955);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[419], 2863265193);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[420], 4294920957);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[421], 4294920953);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[422], 3435927499);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[423], 2576934040);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[424], 4294874618);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[425], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[426], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[427], 4294920951);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[428], 3435927497);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[429], 2576934038);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[430], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[431], 4294874616);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[432], 46338);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[433], 2863357872);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[434], 1431702107);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[435], 46336);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[436], 3436020178);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[437], 2577026719);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[438], 92681);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[439], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[440], 46339);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[441], 2863357873);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[442], 46341);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[443], 46337);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[444], 3436020179);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[445], 2577026720);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[446], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[447], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[448], 1431702106);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[449], 46335);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[450], 3436020177);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[451], 2577026718);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[452], 92680);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[453], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[454], 2863311532);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[455], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[456], 4294967292);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[457], 3435973838);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[458], 2576980379);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[459], 4294920957);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[460], 2863311530);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[461], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[462], 4294967290);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[463], 3435973836);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[464], 2576980377);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[465], 46339);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[466], 4294920955);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[467], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[468], 2863311535);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[469], 1431655770);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[470], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[471], 3435973841);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[472], 2576980382);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[473], 46344);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[474], 4294920960);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[475], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[476], 2863311536);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[477], 4);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[478], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[479], 3435973842);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[480], 2576980383);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[481], 4294920961);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[482], 2863311534);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[483], 1431655769);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[484], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[485], 3435973840);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[486], 2576980381);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[487], 46343);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[488], 4294920959);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[489], 858993455);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[490], 3722304989);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[491], 2290649224);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[492], 858993453);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[493], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[494], 3435973836);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[495], 859039798);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[496], 858947118);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[497], 858993456);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[498], 3722304990);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[499], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[500], 858993454);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[501], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[502], 3435973837);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[503], 858947119);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[504], 3722304988);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[505], 2290649223);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[506], 858993452);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[507], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[508], 3435973835);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[509], 859039797);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[510], 858947117);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[511], 1717986914);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[512], 286331152);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[513], 3149642683);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[514], 1717986912);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[515], 858993458);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[516], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[517], 1718033257);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[518], 1717940577);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[519], 1717986915);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[520], 286331153);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[521], 1717986917);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[522], 1717986913);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[523], 858993459);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[524], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[525], 1717940578);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[526], 286331151);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[527], 3149642682);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[528], 1717986911);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[529], 858993457);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[530], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[531], 1718033256);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[532], 1717940576);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[533], 46336);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[534], 2863357870);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[535], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[536], 46334);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[537], 3436020176);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[538], 2577026717);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[539], 92679);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[540], 4294967295);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[541], 46337);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[542], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[543], 46339);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[544], 46335);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[545], 3436020177);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[546], 2577026718);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[547], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[548], 2863357869);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[549], 1431702104);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[550], 46333);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[551], 3436020175);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[552], 2577026716);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[553], 92678);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[554], 4294967294);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[555], 1431655763);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[556], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[557], 2863311532);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[558], 1431655761);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[559], 572662307);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[560], 4008636144);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[561], 1431702106);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[562], 1431609426);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[563], 1431655764);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[564], 2);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[565], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[566], 1431655762);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[567], 572662308);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[568], 4008636145);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[569], 1431609427);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[570], 0);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[571], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[572], 1431655760);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[573], 572662306);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[574], 4008636143);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[575], 1431702105);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[576], 1431609425);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[577], 2863311528);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[578], 1431655766);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[579], 1);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[580], 2863311526);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[581], 2004318072);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[582], 1145324613);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[583], 2863357871);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[584], 2863265191);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[585], 2863311529);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[586], 1431655767);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[587], 2863311531);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[588], 2863311527);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[589], 2004318073);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[590], 1145324614);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[591], 2863265192);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[592], 1431655765);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x7___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[593], 122);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[594], 32);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val_0x9___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[595], 4294967177);
}


TEST_F(sub_01, opcode__sub___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[596], 2147481600);
}

