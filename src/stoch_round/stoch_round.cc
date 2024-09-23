// Standard C++ includes
#include <fstream>
#include <random>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V common includes
#include "common/app_utils.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"


CodeGenerator generateCode(unsigned int numVectorMultiply, uint32_t seedPointer, 
                           uint32_t inputPtr, uint32_t resPointer)
{
    CodeGenerator c;
    VectorRegisterAllocator vectorRegisterAllocator;
    ScalarRegisterAllocator scalarRegisterAllocator;

    // Register allocation
    ALLOCATE_SCALAR(SInBuffer);
    ALLOCATE_SCALAR(SInBufferEnd);
    ALLOCATE_SCALAR(SResBuffer);
    
    // Labels
    Label mulLoop;

    // Load RNG seed
    {
        ALLOCATE_SCALAR(STmp);
        c.li(*STmp, seedPointer);
        c.vloadr0(*STmp);
        c.vloadr1(*STmp, 64);
    }

    c.li(*SInBuffer, inputPtr);
    c.li(*SInBufferEnd, inputPtr + (128 * numVectorMultiply));

    c.li(*SResBuffer, resPointer);

    // Loop over time
    c.L(mulLoop);
    {
        // Register allocation
        ALLOCATE_VECTOR(VA);
        ALLOCATE_VECTOR(VB);
        ALLOCATE_VECTOR(VRz);
        ALLOCATE_VECTOR(VRn);
        ALLOCATE_VECTOR(VRs);

        // Load operands
        c.vloadv(*VA, *SInBuffer, 0);
        c.vloadv(*VB, *SInBuffer, 64);
        c.addi(*SInBuffer, *SInBuffer, 64 * 2);

        // Multiply operands using each instruction
        c.vmul(15, *VRz, *VA, *VB);
        c.vmul_rn(15, *VRn, *VA, *VB);
        c.vmul_rs(15, *VRs, *VA, *VB);
        
        // Store operands
        c.vstore(*VRz, *SResBuffer);
        c.vstore(*VRn, *SResBuffer, 64);
        c.vstore(*VRs, *SResBuffer, 128);
        c.addi(*SResBuffer, *SResBuffer, 64 * 3);

        // While x2 (address) < x1 (count), goto loop
        c.bne(*SInBuffer, *SInBufferEnd, mulLoop);
    }
    
    c.ecall();
    return c;
}


int main()
{
    const unsigned int numVectorMultiply = 1562;

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Generate seed
    const uint32_t seedPointer = AppUtils::allocateVectorSeedAndInit(vectorInitData);

    // Load operand  data into vector memory
    const uint32_t inputPtr = AppUtils::loadVectors("stoch_rand_operands.bin", vectorInitData);
    
    // Allocate memory to store multiplication results
    const uint32_t resPointer = AppUtils::allocateVectorAndZero(32 * 3 * numVectorMultiply, vectorInitData);
    
    // Create RISC-V core with instruction and scalar data
    RISCV riscV(generateCode(numVectorMultiply, seedPointer, inputPtr, resPointer).getCode(), scalarInitData);
    
    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
    // Run!
    riscV.run();
    
    const auto *vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData().data();
    
    // Write results to binary file
    std::ofstream out("out_stoch_round.bin", std::ios::binary);
    out.write(reinterpret_cast<const char*>(vectorData + (resPointer / 2)),
              32 * 3 * numVectorMultiply * 2);
    
    

}
