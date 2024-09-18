// Standard C++ includes
#include <fstream>
#include <numeric>

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

CodeGenerator generateCode(double tauM, double tauA, uint32_t poissonPtr, 
                           uint32_t outputPointer, size_t numTimesteps)
{
    CodeGenerator c;
    VectorRegisterAllocator vectorRegisterAllocator;
    ScalarRegisterAllocator scalarRegisterAllocator;

    // Generate code to copy vector of currents from scalar memory to vector memory
    //AppUtils::generateScalarVectorMemCpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
    //                                     0, 0, 1);

    // Register allocation
    ALLOCATE_SCALAR(SPoissonBuffer);
    ALLOCATE_SCALAR(SVBuffer);
    ALLOCATE_SCALAR(SVBufferEnd);
    ALLOCATE_VECTOR(VAlpha);
    ALLOCATE_VECTOR(VRho);
    ALLOCATE_VECTOR(VWeight);
    ALLOCATE_VECTOR(VBeta);
    ALLOCATE_VECTOR(VV);
    ALLOCATE_VECTOR(VA);
    ALLOCATE_VECTOR(VVThresh);
    ALLOCATE_VECTOR(VOne);
    ALLOCATE_VECTOR(VI);

    // Calculate constants
    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / tauM), 14));
    c.vlui(*VRho, convertFixedPoint(std::exp(-1.0 / tauA), 14));
    c.vlui(*VWeight, convertFixedPoint(0.01, 10));
    c.vlui(*VBeta, convertFixedPoint(0.0174, 10));
    c.vlui(*VV, 0);
    c.vlui(*VA, 0);
    c.vlui(*VVThresh, convertFixedPoint(0.6, 10));
    c.vlui(*VOne, convertFixedPoint(1.0, 10));

    // Start reading at poisson pointer
    c.li(*SPoissonBuffer, poissonPtr);

    // Start writing 64 bytes in (after I values)
    c.li(*SVBuffer, outputPointer);

    // End writing at 100 timesteps * 64 bytes
    c.li(*SVBufferEnd, outputPointer + (64 * numTimesteps));
    
    // Loop over time
    Label loop;
    c.L(loop);
    {
        // Register allocation
        ALLOCATE_SCALAR(SSpike);

        // i = *poissonPointer
        c.vloadv(*VI, *SPoissonBuffer);
        c.addi(*SPoissonBuffer, *SPoissonBuffer, 64);
        c.vmul(0, *VI, *VI, *VWeight);
   
        // v *= alpha
        c.vmul(14, *VV, *VV, *VAlpha);
    
        // v += i
        c.vadd(*VV, *VV, *VI);

        // a *= rho
        c.vmul(14, *VA, *VA, *VRho);
    
        // spike = VV >= (VThres + (Beta * A))
        {
            ALLOCATE_VECTOR(VTmp);
            c.vmul(10, *VTmp, *VA, *VBeta);
            c.vadd(*VTmp, *VTmp, *VVThresh);
            c.vtge(*SSpike, *VV, *VTmp);
        }
        
        {
            ALLOCATE_VECTOR(VTmp1);
            ALLOCATE_VECTOR(VTmp2);
            c.vsub(*VTmp1, *VV, *VVThresh);
            c.vadd(*VTmp2, *VA, *VOne);

            // v = spk ? (v - v_thresh) : v
            c.vsel(*VV, *SSpike, *VTmp1);

            // a = spk ? (a + 1) : a
            c.vsel(*VA, *SSpike, *VTmp2);
        }
        
    
        //vmem[a...a+32] = v
        c.vstore(*VV, *SVBuffer);
        c.addi(*SVBuffer, *SVBuffer, 64);
    
        // While x2 (address) < x1 (count), goto loop
        c.bne(*SVBuffer, *SVBufferEnd, loop);
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
    std::vector<int16_t> vectorInitData;
    
    // Load poisson data into vector memory
    const uint32_t poissonPtr = AppUtils::loadVectors("poisson_data.bin", vectorInitData);
    
    // Allocate memory to store neuron voltages
    const size_t numTimesteps = 1370;
    const uint32_t outputPointer = AppUtils::allocateVectorAndZero(32 * numTimesteps, vectorInitData);

    // Generate code
    const auto code = generateCode(20.0, 2000.0, poissonPtr, outputPointer, numTimesteps).getCode();

    // Dump to coe file
    AppUtils::dumpCOE("alif.coe", code);

    // Create RISC-V core with instruction and scalar data
    RISCV riscV(code, scalarInitData);
    
    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
    // Run!
    riscV.run();
    
    // Get pointer to vector memory
    auto *vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData().data();
    
    // From these, get pointers to data structures
    int16_t *outputVSum = vectorData + (outputPointer / 2);

    std::ofstream out("out_alif.txt");
    for(int t = 0; t < numTimesteps; t++) {
        for(int l = 0; l < 32; l++) {
            out << *outputVSum++;
            if(l != 31) {
                out << ", ";
            }
        }
        out << std::endl;
        
    }
    

}
