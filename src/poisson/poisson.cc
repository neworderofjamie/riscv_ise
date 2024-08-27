// Standard C++ includes
#include <fstream>
#include <random>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V assembler includes
#include "assembler/register_allocator.h"
#include "assembler/xbyak_riscv.hpp"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"


Xbyak_riscv::CodeGenerator generateCode()
{
    using namespace Xbyak_riscv;
    
    CodeGenerator c;
    VectorRegisterAllocator vectorRegisterAllocator;
    ScalarRegisterAllocator scalarRegisterAllocator;

    // Register allocation
    ALLOCATE_SCALAR(SIBuffer);
    ALLOCATE_SCALAR(SIBufferEnd);
    ALLOCATE_VECTOR(VExpMinusLambda);
    ALLOCATE_VECTOR(VI);

    // Labels
    Label timeLoop;
    Label poissonLoop;

    // Load RNG seed from first 128 bytes of vector memory
    c.vloadr0(Reg::X0);
    c.vloadr1(Reg::X0, 64);

    c.vlui(*VExpMinusLambda, convertFixedPoint(std::exp(-20.0 * 256 / 1000.0), 14));

    // Start writing at start
    c.li(*SIBuffer, 0);

    // End writing at 100 timesteps * 64 bytes
    c.li(*SIBufferEnd, 100 * 32 * 2);

    // Loop over time
    c.L(timeLoop);
    {
        ALLOCATE_SCALAR(SMask);
        ALLOCATE_VECTOR(VNumSpikes);
        ALLOCATE_VECTOR(VP);
        ALLOCATE_VECTOR(VOne);
        
        c.vlui(*VNumSpikes, 0);
        c.vlui(*VP, convertFixedPoint(1.0, 14));
        c.vlui(*VOne, 1);
        c.li(*SMask, 0xFFFFFFFF);
        c.L(poissonLoop);
        {
            ALLOCATE_VECTOR(VNewNumSpikes);
            ALLOCATE_SCALAR(SNewMask);
            ALLOCATE_VECTOR(VNewP);
            ALLOCATE_VECTOR(VRand);

            // Generate uniformly distributed random number
            c.vrng(*VRand);

            // NumSpikes++
            c.vadd(*VNewNumSpikes, *VNumSpikes, *VOne);

            // P *= VRand
            c.vmul(15, *VNewP, *VP, *VRand);

            c.vsel(*VNumSpikes, *SMask, *VNewNumSpikes);
            c.vsel(*VP, *SMask, *VNewP);

            //SNewMask = ExpMinusLambda < p
            c.vtlt(*SNewMask, *VExpMinusLambda, *VNewP);
            c.and_(*SMask, *SMask, *SNewMask);

            c.bne(*SMask, Reg::X0, poissonLoop);
        }
        
        c.vsub(*VI, *VNumSpikes, *VOne);

        //vmem[a...a+32] = v
        c.vstore(*VI, *SIBuffer);
        c.addi(*SIBuffer, *SIBuffer, 64);
    
        // While x2 (address) < x1 (count), goto loop
        c.bne(*SIBuffer, *SIBufferEnd, timeLoop);
    }
    
    c.ecall();
    return c;
}


int main()
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData(3200);
    
    // Fill first 64 half words of vector memory with random seed data
    std::random_device seedSource;
    for(size_t i = 0; i < 32; i++) {
        const uint32_t seed = seedSource();
        vectorInitData[(i * 2) + 0] =  seed & 0xFFFF;
        vectorInitData[(i * 2) + 1] = (seed >> 16) & 0xFFFF;
    }
        
    // Create RISC-V core with instruction and scalar data
    RISCV riscV(generateCode().getCode(), scalarInitData);
    
    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
    // Run!
    riscV.run();
    
    const auto &vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData();
    
    std::ofstream out("out_poisson.txt");
    for(int16_t v : vectorData) {
        out << v << std::endl;
    }
    

}
