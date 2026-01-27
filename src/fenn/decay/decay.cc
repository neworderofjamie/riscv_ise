// Standard C++ includes
#include <fstream>
#include <random>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"


CodeGenerator generateCode(double tau, unsigned int numTimesteps)
{
    CodeGenerator c;
    VectorRegisterAllocator vectorRegisterAllocator;
    ScalarRegisterAllocator scalarRegisterAllocator;

    // Register allocation
    ALLOCATE_SCALAR(SVBuffer);
    ALLOCATE_SCALAR(SVBufferEnd);
    ALLOCATE_VECTOR(VDecay);
    ALLOCATE_VECTOR(VZ);
    ALLOCATE_VECTOR(VN);
    ALLOCATE_VECTOR(VS);

    // Labels
    Label timeLoop;

    // Load RNG seed from first 128 bytes of vector memory
    c.vloadr0(Reg::X0);
    c.vloadr1(Reg::X0, 64);

    c.vlui(*VDecay, convertFixedPoint(std::exp(-1.0 / tau), 14));

    c.vlui(*VZ, convertFixedPoint(1.0, 14));
    c.vlui(*VN, convertFixedPoint(1.0, 14));
    c.vlui(*VS, convertFixedPoint(1.0, 14));

    // Start writing at start
    c.li(*SVBuffer, 0);

    // End writing at 3 * 64 bytes for each timestep
    c.li(*SVBufferEnd, numTimesteps * 3 * 32 * 2);

    // Loop over time
    c.L(timeLoop);
    {
        // Decay
        c.vmul(14, *VZ, *VZ, *VDecay);
        c.vmul_rn(14, *VN, *VN, *VDecay);
        c.vmul_rs(14, *VS, *VS, *VDecay);
        
        c.vstore(*VZ, *SVBuffer);
        c.vstore(*VN, *SVBuffer, 64);
        c.vstore(*VS, *SVBuffer, 128);
        c.addi(*SVBuffer, *SVBuffer, 64 * 3);

        // While x2 (address) < x1 (count), goto loop
        c.bne(*SVBuffer, *SVBufferEnd, timeLoop);
    }
    
    c.ecall();
    return c;
}


int main()
{
    const unsigned int numTimesteps = 40000;

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData(numTimesteps * 3 * 32);

    // Fill first 64 half words of vector memory with random seed data
    std::random_device seedSource;
    for(size_t i = 0; i < 32; i++) {
        const uint32_t seed = seedSource();
        vectorInitData[(i * 2) + 0] =  seed & 0xFFFF;
        vectorInitData[(i * 2) + 1] = (seed >> 16) & 0xFFFF;
    }

    // Create RISC-V core with instruction and scalar data
    RISCV riscV(generateCode(5000.0, numTimesteps).getCode(), scalarInitData);
    
    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
    // Run!
    riscV.run();
    
    const auto &vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData();
    
    std::ofstream out("out_decay.txt");
    for(int16_t v : vectorData) {
        out << v << std::endl;
    }
    

}
