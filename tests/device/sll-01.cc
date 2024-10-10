// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V commonincludes
#include "common/device.h"

// RISC-V architecture test includes
#include "riscv_arch_test/sll-01.h"

namespace
{
class sll_01 : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = ArchTest::sll_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Disable device, upload code and data
        Device device;
        device.setEnabled(false);
        device.uploadCode(ArchTest::sll_01::generateCode(false));
        device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());
        
        // Re-enable 
        device.setEnabled(true);
        
        // Wait for tests to complete
        device.waitOnNonZero(0);
        
        // Disable and copy data
        device.setEnabled(false);
        s_ScalarData.resize(scalarInitData.size());
        device.memcpyDataFromDevice(s_ScalarData.data(), 0, scalarInitData.size());
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

std::vector<uint8_t> sll_01::s_ScalarData;
}


// Generate google tests

TEST_F(sll_01, opcode__sll___op1_x6__op2_x13__dest_x1__op1val__0x40000000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[5], 3221225472);
}


TEST_F(sll_01, opcode__sll___op1_x4__op2_x1__dest_x4__op1val__0x11___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[6], 4294410240);
}


TEST_F(sll_01, opcode__sll___op1_x9__op2_x11__dest_x11__op1val__0x101___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[7], 2139095040);
}


TEST_F(sll_01, opcode__sll___op1_x12__op2_x12__dest_x9__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[8], 3221225472);
}


TEST_F(sll_01, opcode__sll___op1_x0__op2_x0__dest_x0__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(sll_01, opcode__sll___op1_x5__op2_x4__dest_x15__op1val__0x41___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[10], 3221225472);
}


TEST_F(sll_01, opcode__sll___op1_x1__op2_x10__dest_x3__op1val_0x7fffffff___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[11], 4294966784);
}


TEST_F(sll_01, opcode__sll___op1_x8__op2_x15__dest_x10__op1val__0x40000001___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[12], 4294836224);
}


TEST_F(sll_01, opcode__sll___op1_x13__op2_x6__dest_x12__op1val__0x20000001___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[13], 3221225470);
}


TEST_F(sll_01, opcode__sll___op1_x3__op2_x9__dest_x5__op1val__0x10000001___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[14], 4294950912);
}


TEST_F(sll_01, opcode__sll___op1_x7__op2_x2__dest_x13__op1val__0x8000001___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[15], 3221225472);
}


TEST_F(sll_01, opcode__sll___op1_x11__op2_x7__dest_x8__op1val__0x4000001___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[16], 4286578688);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x14__dest_x7__op1val__0x2000001___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[17], 4294967040);
}


TEST_F(sll_01, opcode__sll___op1_x14__op2_x8__dest_x6__op1val__0x1000001___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[18], 4294959104);
}


TEST_F(sll_01, opcode__sll___op1_x15__op2_x3__dest_x2__op1val__0x800001___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[19], 4292870144);
}


TEST_F(sll_01, opcode__sll___op1_x2__op2_x5__dest_x14__op1val__0x400001___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[20], 4294965248);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[21], 4294965248);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[22], 4290772988);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val_0x9) {
  ASSERT_EQ(getScalarWordData()[23], 4026531328);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[24], 4294934528);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x20001___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[25], 4160748544);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[26], 4294705148);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[27], 4294442992);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[28], 4294950911);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val_0xd) {
  ASSERT_EQ(getScalarWordData()[29], 4227850240);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val_0x1e) {
  ASSERT_EQ(getScalarWordData()[30], 3221225472);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[31], 4160684032);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[32], 4294934496);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[33], 4294950880);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x81___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[34], 4286513152);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[35], 4292804608);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[36], 4294377472);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val_0xf) {
  ASSERT_EQ(getScalarWordData()[37], 4294803456);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[38], 4294967248);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[39], 4294967288);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[40], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[41], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0xb) {
  ASSERT_EQ(getScalarWordData()[42], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[43], 536870912);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[44], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[45], 2147483648);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[46], 268435456);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[47], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[48], 134217728);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[49], 4194304);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[50], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[51], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[52], 67108864);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[53], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[54], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x10000___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[55], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[56], 134217728);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x4000___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[57], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[58], 2097152);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[59], 4194304);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[60], 2097152);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[61], 134217728);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[62], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[63], 32768);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[64], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x40___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[65], 65536);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[66], 32768);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val_0x17) {
  ASSERT_EQ(getScalarWordData()[67], 134217728);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[68], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[69], 268435456);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x1___op2val_0x1f) {
  ASSERT_EQ(getScalarWordData()[70], 2147483648);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[71], 2821193728);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[72], 4283104512);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[73], 2576980380);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[74], 1717986944);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[75], 786432);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[76], 1431699456);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x13) {
  ASSERT_EQ(getScalarWordData()[77], 2863661056);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[78], 46339);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x11) {
  ASSERT_EQ(getScalarWordData()[79], 393216);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[80], 2863267840);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[81], 1431568384);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xc) {
  ASSERT_EQ(getScalarWordData()[82], 0);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[83], 8);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[84], 1717986560);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xe) {
  ASSERT_EQ(getScalarWordData()[85], 3435954176);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[86], 1431655764);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x12) {
  ASSERT_EQ(getScalarWordData()[87], 3557818368);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x15) {
  ASSERT_EQ(getScalarWordData()[88], 3435134976);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[89], 3435973824);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xa) {
  ASSERT_EQ(getScalarWordData()[90], 5120);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x1b) {
  ASSERT_EQ(getScalarWordData()[91], 3758096384);
}


TEST_F(sll_01, opcode__sll___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x1d) {
  ASSERT_EQ(getScalarWordData()[92], 2147483648);
}

