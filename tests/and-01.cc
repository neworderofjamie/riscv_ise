// Standard C includes
#include <cassert>
#include <cstring>

// Google test includes
#include "gtest/gtest.h"

// RISC-V ISE includes
#include "ise/riscv.h"

// RISC-V architecture test includes
#include "riscv_arch_test/and-01.h"

namespace
{
class and_01Fixture : public testing::Test 
{
protected:
    // Per-test-suite set-up.
    // Called before the first test in this test suite.
    // Can be omitted if not needed.
    static void SetUpTestSuite() 
    {
        const auto &scalarWordInitData = and_01::getScalarInitData();
        
        std::vector<uint8_t> scalarInitData(scalarWordInitData.size() * 4);
        std::memcpy(scalarInitData.data(), scalarWordInitData.data(),
                    scalarWordInitData.size() * 4);

        // Create RISC-V core with instruction and scalar data
        RISCV riscV(and_01::generateCode().getCode(), scalarInitData);
        
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

std::vector<uint8_t> and_01Fixture::s_ScalarData;
}


// Generate google tests

TEST_F(and_01Fixture, opcode__and___op1_x24__op2_x25__dest_x25__op1val_0x4000___op2val_0x7) {
  ASSERT_EQ(getScalarWordData()[4], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x3__op2_x3__dest_x18__op1val_0x800___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[5], 2048);
}


TEST_F(and_01Fixture, opcode__and___op1_x19__op2_x19__dest_x19__op1val__0x3___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[6], 4294967293);
}


TEST_F(and_01Fixture, opcode__and___op1_x5__op2_x14__dest_x5__op1val__0x1___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[7], 2147483647);
}


TEST_F(and_01Fixture, opcode__and___op1_x23__op2_x16__dest_x20__op1val_0x5___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[8], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x20__op2_x2__dest_x30__op1val_0x2___op2val__0x80000000) {
  ASSERT_EQ(getScalarWordData()[9], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x7__op2_x24__dest_x13__op1val_0x33333333___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[10], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x30__op2_x27__dest_x10__op1val__0x40000001___op2val_0x1) {
  ASSERT_EQ(getScalarWordData()[11], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x28__op2_x18__dest_x22__op1val__0x80000000___op2val_0x800) {
  ASSERT_EQ(getScalarWordData()[12], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x2__op2_x15__dest_x0__op1val_0x0___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[13], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x25__op2_x26__dest_x12__op1val_0x7fffffff___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[14], 1431655765);
}


TEST_F(and_01Fixture, opcode__and___op1_x1__op2_x31__dest_x2__op1val_0x1___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[15], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x27__op2_x11__dest_x14__op1val_0x40000___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[16], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x31__op2_x23__dest_x4__op1val__0x20001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[17], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x21__op2_x9__dest_x27__op1val__0x55555555___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[18], 8);
}


TEST_F(and_01Fixture, opcode__and___op1_x26__op2_x7__dest_x23__op1val_0x400___op2val_0x10) {
  ASSERT_EQ(getScalarWordData()[19], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x9__op2_x20__dest_x24__op1val__0x8___op2val_0x20) {
  ASSERT_EQ(getScalarWordData()[20], 32);
}


TEST_F(and_01Fixture, opcode__and___op1_x15__op2_x13__dest_x26__op1val__0x101___op2val_0x40) {
  ASSERT_EQ(getScalarWordData()[21], 64);
}


TEST_F(and_01Fixture, opcode__and___op1_x12__op2_x4__dest_x17__op1val__0x2000001___op2val_0x80) {
  ASSERT_EQ(getScalarWordData()[22], 128);
}


TEST_F(and_01Fixture, opcode__and___op1_x4__op2_x17__dest_x8__op1val_0x66666665___op2val_0x100) {
  ASSERT_EQ(getScalarWordData()[23], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x13__op2_x22__dest_x1__op1val__0x81___op2val_0x400) {
  ASSERT_EQ(getScalarWordData()[24], 1024);
}


TEST_F(and_01Fixture, opcode__and___op1_x8__op2_x12__dest_x7__op1val_0x4___op2val_0x1000) {
  ASSERT_EQ(getScalarWordData()[25], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x11__op2_x5__dest_x21__op1val__0x8001___op2val_0x2000) {
  ASSERT_EQ(getScalarWordData()[26], 8192);
}


TEST_F(and_01Fixture, opcode__and___op1_x14__op2_x21__dest_x29__op1val_0x40___op2val_0x4000) {
  ASSERT_EQ(getScalarWordData()[27], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x17__op2_x10__dest_x11__op1val_0x5___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[28], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x22__op2_x6__dest_x3__op1val_0x800000___op2val_0x10000) {
  ASSERT_EQ(getScalarWordData()[29], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x0__op2_x29__dest_x9__op1val_0x0___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[30], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x29__op2_x30__dest_x28__op1val_0x5___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[31], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x18__op2_x1__dest_x6__op1val_0x10000___op2val_0x80000) {
  ASSERT_EQ(getScalarWordData()[32], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x6__op2_x0__dest_x31__op1val_0x8000000___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[33], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x16__op2_x8__dest_x15__op1val__0x3___op2val_0x200000) {
  ASSERT_EQ(getScalarWordData()[34], 2097152);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x28__dest_x16__op1val_0x800___op2val_0x400000) {
  ASSERT_EQ(getScalarWordData()[35], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x800000) {
  ASSERT_EQ(getScalarWordData()[36], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x1000000) {
  ASSERT_EQ(getScalarWordData()[37], 16777216);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2000000) {
  ASSERT_EQ(getScalarWordData()[38], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val_0x4000000) {
  ASSERT_EQ(getScalarWordData()[39], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x8001___op2val_0x8000000) {
  ASSERT_EQ(getScalarWordData()[40], 134217728);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x10000000) {
  ASSERT_EQ(getScalarWordData()[41], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x10001___op2val_0x20000000) {
  ASSERT_EQ(getScalarWordData()[42], 536870912);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x40000000) {
  ASSERT_EQ(getScalarWordData()[43], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val__0x2) {
  ASSERT_EQ(getScalarWordData()[44], 4294966782);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x40000001___op2val__0x3) {
  ASSERT_EQ(getScalarWordData()[45], 3221225469);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x800000___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[46], 8388608);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x201___op2val__0x9) {
  ASSERT_EQ(getScalarWordData()[47], 4294966775);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x8000001___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[48], 4160749551);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x21) {
  ASSERT_EQ(getScalarWordData()[49], 3);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x80___op2val__0x41) {
  ASSERT_EQ(getScalarWordData()[50], 128);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x7___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[51], 4294967161);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x9___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[52], 4294967031);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x201) {
  ASSERT_EQ(getScalarWordData()[53], 2863311018);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x800001___op2val__0x401) {
  ASSERT_EQ(getScalarWordData()[54], 4286577663);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x100001___op2val__0x801) {
  ASSERT_EQ(getScalarWordData()[55], 4293916671);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val__0x1001) {
  ASSERT_EQ(getScalarWordData()[56], 1024);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[57], 1717978726);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x7___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[58], 7);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2000000___op2val__0x8001) {
  ASSERT_EQ(getScalarWordData()[59], 33554432);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x40000000___op2val__0x10001) {
  ASSERT_EQ(getScalarWordData()[60], 1073741824);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x5___op2val__0x20001) {
  ASSERT_EQ(getScalarWordData()[61], 4294836219);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x40000___op2val__0x40001) {
  ASSERT_EQ(getScalarWordData()[62], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x10___op2val__0x80001) {
  ASSERT_EQ(getScalarWordData()[63], 16);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x400___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[64], 1024);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x1___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[65], 4290772991);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x20000___op2val__0x800001) {
  ASSERT_EQ(getScalarWordData()[66], 131072);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x40001___op2val__0x1000001) {
  ASSERT_EQ(getScalarWordData()[67], 4277927935);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x2000001) {
  ASSERT_EQ(getScalarWordData()[68], 6);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x4000001) {
  ASSERT_EQ(getScalarWordData()[69], 2863311531);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x11___op2val__0x8000001) {
  ASSERT_EQ(getScalarWordData()[70], 4160749551);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x20000001___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[71], 3489660927);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x20000001) {
  ASSERT_EQ(getScalarWordData()[72], 3758050045);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x80000000___op2val__0x40000001) {
  ASSERT_EQ(getScalarWordData()[73], 2147483648);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[74], 2863270568);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x8___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[75], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x20___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[76], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x100___op2val__0x101) {
  ASSERT_EQ(getScalarWordData()[77], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x200___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[78], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x1000___op2val_0x8) {
  ASSERT_EQ(getScalarWordData()[79], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2000___op2val_0x40000) {
  ASSERT_EQ(getScalarWordData()[80], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x8000___op2val__0x4) {
  ASSERT_EQ(getScalarWordData()[81], 32768);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x80000___op2val_0x3fffffff) {
  ASSERT_EQ(getScalarWordData()[82], 524288);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x100000___op2val__0x2001) {
  ASSERT_EQ(getScalarWordData()[83], 1048576);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x200000___op2val_0x8000) {
  ASSERT_EQ(getScalarWordData()[84], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x400000___op2val__0x11) {
  ASSERT_EQ(getScalarWordData()[85], 4194304);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x1000000___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[86], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4000000___op2val_0x20000) {
  ASSERT_EQ(getScalarWordData()[87], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x10000000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[88], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x20000000___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[89], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x2___op2val__0x10000001) {
  ASSERT_EQ(getScalarWordData()[90], 4026531838);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x21___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[91], 858993426);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x41___op2val__0x400001) {
  ASSERT_EQ(getScalarWordData()[92], 4290772927);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x401___op2val__0x4001) {
  ASSERT_EQ(getScalarWordData()[93], 4294949887);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x801___op2val__0x7) {
  ASSERT_EQ(getScalarWordData()[94], 4294965241);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x1001___op2val__0x81) {
  ASSERT_EQ(getScalarWordData()[95], 4294963071);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x2001___op2val__0x5) {
  ASSERT_EQ(getScalarWordData()[96], 4294959099);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x4001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[97], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x80001___op2val__0x200001) {
  ASSERT_EQ(getScalarWordData()[98], 4292345855);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x200001___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[99], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x400001___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[100], 858993460);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x1000001___op2val_0x7fffffff) {
  ASSERT_EQ(getScalarWordData()[101], 2130706431);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x4000001___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[102], 1650878053);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x10000001___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[103], 4026485501);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[104], 3);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[105], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[106], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[107], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[108], 3);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[109], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[110], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[111], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[112], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[113], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[114], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[115], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[116], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[117], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[118], 3);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[119], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[120], 3);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[121], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[122], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[123], 3);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[124], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x3___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[125], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[126], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[127], 1431655765);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[128], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[129], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[130], 286331153);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[131], 1145324612);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[132], 1431650388);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[133], 5380);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[134], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[135], 1431655764);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[136], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[137], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[138], 286331152);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[139], 1145324613);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[140], 5377);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[141], 1431655764);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[142], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[143], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[144], 286331156);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[145], 1145324613);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[146], 1431650389);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[147], 5381);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[148], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[149], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[150], 2863311530);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[151], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[152], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[153], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[154], 2863270568);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[155], 40960);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[156], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[157], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[158], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[159], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[160], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[161], 572662304);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[162], 40962);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[163], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[164], 2863311530);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[165], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[166], 572662304);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[167], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[168], 2863270568);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[169], 40960);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[170], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[171], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[172], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[173], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[174], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[175], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[176], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[177], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[178], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[179], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[180], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[181], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[182], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[183], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[184], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[185], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[186], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[187], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[188], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[189], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x5___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[190], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[191], 3);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[192], 286331153);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[193], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[194], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[195], 858993459);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[196], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[197], 858980912);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[198], 12544);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[199], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[200], 286331152);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[201], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[202], 858993458);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[203], 572662305);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[204], 12547);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[205], 286331154);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[206], 572662307);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[207], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[208], 858993456);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[209], 572662307);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[210], 858980913);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333333___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[211], 12545);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[212], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[213], 1145324612);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[214], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[215], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[216], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[217], 1717986918);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[218], 1717977700);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[219], 9220);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[220], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[221], 1145324612);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[222], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[223], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[224], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[225], 1717986916);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[226], 9218);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[227], 1145324614);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[228], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[229], 6);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[230], 572662308);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[231], 1717986918);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[232], 1717977700);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666666___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[233], 9220);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[234], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[235], 1431650388);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[236], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[237], 858980912);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[238], 1717977700);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[239], 4294920956);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[240], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[241], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[242], 1431650388);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[243], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[244], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[245], 858980912);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[246], 1717977700);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[247], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[248], 1431650388);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[249], 2863270568);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[250], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[251], 858980916);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[252], 1717977700);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[253], 4294920956);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[254], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[255], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[256], 5380);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[257], 40960);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[258], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[259], 12544);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[260], 9220);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[261], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[262], 46340);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[263], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[264], 5380);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[265], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[266], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[267], 12544);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[268], 9220);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[269], 46336);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[270], 5380);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[271], 40960);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[272], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[273], 12548);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[274], 9220);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[275], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb504___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[276], 46340);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[277], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[278], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[279], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[280], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[281], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[282], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[283], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[284], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[285], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[286], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[287], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[288], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[289], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[290], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[291], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[292], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[293], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[294], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[295], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[296], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[297], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x2___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[298], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[299], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[300], 1431655764);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[301], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[302], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[303], 286331152);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[304], 1145324612);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[305], 1431650388);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[306], 5380);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[307], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[308], 1431655764);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[309], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[310], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[311], 286331152);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[312], 1145324612);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[313], 5376);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[314], 1431655764);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[315], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[316], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[317], 286331156);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[318], 1145324612);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[319], 1431650388);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555554___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[320], 5380);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[321], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[322], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[323], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[324], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[325], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[326], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[327], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[328], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[329], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[330], 2863311531);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[331], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[332], 572662304);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[333], 572662307);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[334], 2863270569);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[335], 40961);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[336], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[337], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[338], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[339], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[340], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[341], 6);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[342], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[343], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[344], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[345], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[346], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[347], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[348], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[349], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[350], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[351], 6);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[352], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[353], 6);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[354], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[355], 6);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[356], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x6___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[357], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[358], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[359], 286331156);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[360], 572662304);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[361], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[362], 858993456);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[363], 572662308);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[364], 858980916);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[365], 12548);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[366], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[367], 286331156);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[368], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[369], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[370], 858993456);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[371], 572662308);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[372], 12544);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[373], 286331156);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[374], 572662304);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[375], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[376], 858993460);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[377], 572662308);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[378], 858980916);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333334___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[379], 12548);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[380], 3);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[381], 1145324613);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[382], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[383], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[384], 572662307);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[385], 1717986918);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[386], 1717977700);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[387], 9220);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[388], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[389], 1145324612);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[390], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[391], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[392], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[393], 1717986917);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[394], 9219);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[395], 1145324614);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[396], 572662307);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[397], 6);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[398], 572662308);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[399], 1717986919);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[400], 1717977701);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666667___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[401], 9221);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[402], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[403], 1431650389);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[404], 2863270568);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[405], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[406], 858980913);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[407], 1717977700);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[408], 4294920956);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[409], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[410], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[411], 1431650388);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[412], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[413], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[414], 858980912);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[415], 1717977701);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[416], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[417], 1431650388);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[418], 2863270569);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[419], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[420], 858980916);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[421], 1717977701);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[422], 4294920957);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[423], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[424], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[425], 5381);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[426], 40960);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[427], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[428], 12545);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[429], 9220);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[430], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[431], 46340);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[432], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[433], 5380);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[434], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[435], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[436], 12544);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[437], 9221);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[438], 46337);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[439], 5380);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[440], 40961);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[441], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[442], 12548);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[443], 9221);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[444], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb505___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[445], 46341);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[446], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[447], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[448], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[449], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[450], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[451], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[452], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[453], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[454], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[455], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[456], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[457], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[458], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[459], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[460], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[461], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[462], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[463], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[464], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[465], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[466], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[467], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[468], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[469], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[470], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[471], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[472], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[473], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[474], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[475], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[476], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[477], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[478], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[479], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x4___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[480], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[481], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[482], 286331152);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[483], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[484], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[485], 858993458);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[486], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[487], 858980912);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[488], 12544);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[489], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[490], 286331152);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[491], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[492], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[493], 858993458);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[494], 572662304);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[495], 12546);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[496], 286331154);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[497], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[498], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[499], 858993456);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[500], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[501], 858980912);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x33333332___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[502], 12544);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[503], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[504], 1145324613);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[505], 572662304);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[506], 5);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[507], 572662305);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[508], 1717986916);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[509], 1717977700);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[510], 9220);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[511], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[512], 1145324612);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[513], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[514], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[515], 572662304);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[516], 1717986917);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[517], 9217);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[518], 1145324612);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[519], 572662305);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[520], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[521], 572662308);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[522], 1717986917);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[523], 1717977701);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x66666665___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[524], 9221);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[525], 3);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[526], 5377);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[527], 40962);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[528], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[529], 12547);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[530], 9218);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[531], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[532], 46336);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[533], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[534], 5376);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[535], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[536], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[537], 12546);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[538], 9217);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[539], 46339);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[540], 5378);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[541], 40963);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[542], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[543], 12544);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[544], 9219);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[545], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0xb503___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[546], 46337);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[547], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[548], 1431655764);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[549], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[550], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[551], 286331154);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[552], 1145324614);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[553], 1431650388);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[554], 5380);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[555], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[556], 1431655764);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[557], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[558], 4);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[559], 286331154);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[560], 1145324612);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[561], 5378);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[562], 1431655766);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0x55555555) {
  ASSERT_EQ(getScalarWordData()[563], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x6) {
  ASSERT_EQ(getScalarWordData()[564], 6);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x33333334) {
  ASSERT_EQ(getScalarWordData()[565], 286331156);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0x66666667) {
  ASSERT_EQ(getScalarWordData()[566], 1145324614);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val__0xb503) {
  ASSERT_EQ(getScalarWordData()[567], 1431650388);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x55555556___op2val_0xb505) {
  ASSERT_EQ(getScalarWordData()[568], 5380);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x3) {
  ASSERT_EQ(getScalarWordData()[569], 3);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555555) {
  ASSERT_EQ(getScalarWordData()[570], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0x55555556) {
  ASSERT_EQ(getScalarWordData()[571], 2863311530);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x5) {
  ASSERT_EQ(getScalarWordData()[572], 1);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333333) {
  ASSERT_EQ(getScalarWordData()[573], 572662307);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666666) {
  ASSERT_EQ(getScalarWordData()[574], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val__0xb504) {
  ASSERT_EQ(getScalarWordData()[575], 2863270568);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb504) {
  ASSERT_EQ(getScalarWordData()[576], 40960);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x2) {
  ASSERT_EQ(getScalarWordData()[577], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555554) {
  ASSERT_EQ(getScalarWordData()[578], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x0) {
  ASSERT_EQ(getScalarWordData()[579], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x4) {
  ASSERT_EQ(getScalarWordData()[580], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x33333332) {
  ASSERT_EQ(getScalarWordData()[581], 572662306);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x66666665) {
  ASSERT_EQ(getScalarWordData()[582], 572662305);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0xb503) {
  ASSERT_EQ(getScalarWordData()[583], 40963);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val__0x55555555___op2val_0x55555556) {
  ASSERT_EQ(getScalarWordData()[584], 2);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x800___op2val__0x100001) {
  ASSERT_EQ(getScalarWordData()[585], 2048);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x0___op2val_0x200) {
  ASSERT_EQ(getScalarWordData()[586], 0);
}


TEST_F(and_01Fixture, opcode__and___op1_x10__op2_x11__dest_x12__op1val_0x8000000___op2val_0x100000) {
  ASSERT_EQ(getScalarWordData()[587], 0);
}

