// Standard C++ includes
#include <numeric>

// Standard C includes
#include <cmath>

// Google test includes
#include "gtest/gtest.h"

// RISC-V common includes
#include "common/app_utils.h"

// RISC-V assembler includes
#include "assembler/register_allocator.h"
#include "assembler/xbyak_riscv.hpp"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"

//--------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------
TEST(VSOP, VADD)
{
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    
     std::vector<int16_t> vectorInitData{
        // RS1
        0x7FFF,     0x7FFF,     0x7FFF,     0x7FFF,     0x7FFF,     0x7FFF,     0x7FFF,     0x7FFF,
        0x2AAA,     0x2AAA,     0x2AAA,     0x2AAA,     0x2AAA,     0x2AAA,     0x2AAA,     0x2AAA,
        -0x2AAB,    -0x2AAB,    -0x2AAB,    -0x2AAB,    -0x2AAB,    -0x2AAB,    -0x2AAB,    -0x2AAB,
        -0x8000,    -0x8000,    -0x8000,    -0x8000,    -0x8000,    -0x8000,    -0x8000,    -0x8000,

        // RS2
        0x7FFF,     0x5B6D,     0x36DB,     0x1249,     -0x1249,    -0x36DB,    -0x5B6D,    -0x8000,
        0x7FFF,     0x5B6D,     0x36DB,     0x1249,     -0x1249,    -0x36DB,    -0x5B6D,    -0x8000,
        0x7FFF,     0x5B6D,     0x36DB,     0x1249,     -0x1249,    -0x36DB,    -0x5B6D,    -0x8000,
        0x7FFF,     0x5B6D,     0x36DB,     0x1249,     -0x1249,    -0x36DB,    -0x5B6D,    -0x8000,

        // RS3
        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,        0x0,
     };

    Xbyak_riscv::CodeGenerator c;
    {
        VectorRegisterAllocator vectorRegisterAllocator;
        ScalarRegisterAllocator scalarRegisterAllocator;

        ALLOCATE_SCALAR(SS1);
        ALLOCATE_SCALAR(SS2);
        ALLOCATE_SCALAR(SD);
        ALLOCATE_VECTOR(VS1);
        ALLOCATE_VECTOR(VS2);
        ALLOCATE_VECTOR(VD);

        c.li(*SS1, 0);
        c.li(*SS2, 64);
        c.li(*SD, 96);

        c.vloadv(*VS1, *SS1);
        c.vloadv(*VS2, *SS2);

        c.vadd(*VD, *VS1, *VS2);

        c.vstore(*VD, *SD);

        c.ecall();
     }

    
    // Create RISC-V core with instruction and scalar data
    RISCV riscV(c.getCode(), scalarInitData);
    
    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
    // Run!
    riscV.run();

    auto *vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData().data();
    int16_t *outputVSum = vectorData + 96;
    for(int i = 0; i < 32; i++) {
        std::cout << outputVSum[i] << ", ";
    }
    std::cout << std::endl;
}
