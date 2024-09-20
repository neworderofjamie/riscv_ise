// Standard C++ includes
#include <fstream>
#include <functional>
#include <numeric>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V common includes
#include "common/CLI11.hpp"
#include "common/app_utils.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"

enum class RoundMode
{
    ZERO,
    NEAREST,
    STOCHASTIC,
};

CodeGenerator generateCode(double tauM, double tauA, uint32_t poissonPtr, uint32_t vPointer,  
                           uint32_t aPointer, uint32_t seedPointer, size_t vFixedPoint, 
                           size_t aFixedPoint, size_t numTimesteps, bool saturate, RoundMode roundMode)
{
    CodeGenerator c;
    VectorRegisterAllocator vectorRegisterAllocator;
    ScalarRegisterAllocator scalarRegisterAllocator;

    // Generate code to copy vector of currents from scalar memory to vector memory
    //AppUtils::generateScalarVectorMemCpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
    //                                     0, 0, 1);

    // Register allocation
    ALLOCATE_SCALAR(SPoissonBuffer);
    ALLOCATE_SCALAR(SABuffer);
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

    // Load RNG seed
    {
        ALLOCATE_SCALAR(STmp);
        c.li(*STmp, seedPointer);
        c.vloadr0(*STmp);
        c.vloadr1(*STmp, 64);
    }

    // Calculate constants
    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / tauM), 14));
    c.vlui(*VRho, convertFixedPoint(std::exp(-1.0 / tauA), 14));
    c.vlui(*VWeight, convertFixedPoint(0.01, vFixedPoint));
    c.vlui(*VBeta, convertFixedPoint(0.0174, vFixedPoint));
    c.vlui(*VV, 0);
    c.vlui(*VA, 0);
    c.vlui(*VVThresh, convertFixedPoint(0.6, vFixedPoint));
    c.vlui(*VOne, convertFixedPoint(1.0, aFixedPoint));

    // Start reading at poisson pointer
    c.li(*SPoissonBuffer, poissonPtr);

    // Start writing 64 bytes in (after I values)
    c.li(*SABuffer, aPointer);

    // Start writing 64 bytes in (after I values)
    c.li(*SVBuffer, vPointer);

    // End writing at 100 timesteps * 64 bytes
    c.li(*SVBufferEnd, vPointer + (64 * numTimesteps));
    
    // Pick vadd, vsub and vmul operations to use based on saturation
    const auto vaddFn = std::mem_fn(saturate ? &CodeGenerator::vadd_s : &CodeGenerator::vadd);
    const auto vsubFn = std::mem_fn(saturate ? &CodeGenerator::vsub_s : &CodeGenerator::vsub);
    const auto vmulFn = std::mem_fn((roundMode == RoundMode::NEAREST) ? &CodeGenerator::vmul_rn 
                                    : (roundMode == RoundMode::STOCHASTIC ? &CodeGenerator::vmul_rs 
                                       : &CodeGenerator::vmul));

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
        vmulFn(&c, 14, *VV, *VV, *VAlpha);
    
        // v += i
        vaddFn(&c, *VV, *VV, *VI);

        // a *= rho
        vmulFn(&c, 14, *VA, *VA, *VRho);
    
        // spike = VV >= (VThres + (Beta * A))
        {
            ALLOCATE_VECTOR(VTmp);
            vmulFn(&c, aFixedPoint, *VTmp, *VA, *VBeta);
            vaddFn(&c, *VTmp, *VTmp, *VVThresh);
            c.vtge(*SSpike, *VV, *VTmp);
        }
        
        {
            ALLOCATE_VECTOR(VTmp1);
            ALLOCATE_VECTOR(VTmp2);
            vsubFn(&c, *VTmp1, *VV, *VVThresh);
            vaddFn(&c, *VTmp2, *VA, *VOne);

            // v = spk ? (v - v_thresh) : v
            c.vsel(*VV, *SSpike, *VTmp1);

            // a = spk ? (a + 1) : a
            c.vsel(*VA, *SSpike, *VTmp2);
        }
        
    
        // Write V to memory
        c.vstore(*VV, *SVBuffer);
        c.addi(*SVBuffer, *SVBuffer, 64);

        // Write A to memory
        c.vstore(*VA, *SABuffer);
        c.addi(*SABuffer, *SABuffer, 64);
    
        // While x2 (address) < x1 (count), goto loop
        c.bne(*SVBuffer, *SVBufferEnd, loop);
    }
    
    c.ecall();
    return c;
}


int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    
    
    size_t numTimesteps = 1370;
    size_t aFixedPoint = 10;
    size_t vFixedPoint = 10;
    bool saturate = false;
    RoundMode roundMode = RoundMode::ZERO;
    
    CLI::App app{"ALIF neuron simulation"};
    app.add_option("-t,--timesteps", numTimesteps, "Number of timesteps to simulate");
    app.add_option("-a,--a-fractional-bits", aFixedPoint, "Number of fractional bits to use for A state variable");
    app.add_option("-v,--v-fractional-bits", vFixedPoint, "Number of fractional bits to use for V state variable");
    app.add_flag("-s,--saturate", saturate, "Should saturating operations be used");
    app.add_option("-r,--round-model", roundMode, "What round mode to use");

    CLI11_PARSE(app, argc, argv);


    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;
    
    // Generate seed
    const uint32_t seedPointer = AppUtils::allocateVectorSeedAndInit(vectorInitData);

    // Load poisson data into vector memory
    const uint32_t poissonPtr = AppUtils::loadVectors("poisson_data.bin", vectorInitData);
    
    // Allocate memory to store neuron voltages and adaptation variables
    const uint32_t vPointer = AppUtils::allocateVectorAndZero(32 * numTimesteps, vectorInitData);
    const uint32_t aPointer = AppUtils::allocateVectorAndZero(32 * numTimesteps, vectorInitData);

    // Generate code
    const auto code = generateCode(20.0, 2000.0, poissonPtr, vPointer, aPointer, seedPointer,
                                   vFixedPoint, aFixedPoint, numTimesteps, saturate, roundMode).getCode();

    std::string filenameSuffix = "_" + std::to_string(vFixedPoint) + "_" + std::to_string(aFixedPoint);
    if(saturate) {
        filenameSuffix += "_sat";
    }
    if(roundMode == RoundMode::NEAREST) {
        filenameSuffix += "_rn";
    }
    else if(roundMode == RoundMode::STOCHASTIC) {
        filenameSuffix += "_rs";
    }

    // Dump to coe file
    AppUtils::dumpCOE("alif" + filenameSuffix + ".coe", code);

    // Create RISC-V core with instruction and scalar data
    RISCV riscV(code, scalarInitData);
    
    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
    // Run!
    riscV.run();
    
    // Get pointer to vector memory
    auto *vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData().data();
    
    // From these, get pointers to data structures
    int16_t *outputV = vectorData + (vPointer / 2);
    int16_t *outputA = vectorData + (aPointer / 2);

    std::ofstream out("out_alif" + filenameSuffix + ".txt");
    for(size_t t = 0; t < numTimesteps; t++) {
        for(int l = 0; l < 32; l++) {
            out << *outputV++ << ", " << *outputA++;
            if(l != 31) {
                out << ", ";
            }
        }
        out << std::endl;
    }
}
