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
#include "common/device.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
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

std::vector<uint32_t> generateCode(bool simulate, size_t numTimesteps, bool saturate, RoundMode roundMode, 
                                   double tauM, double tauA, size_t vFixedPoint, size_t aFixedPoint,
                                   uint32_t poissonPtr, uint32_t seedPtr, uint32_t poissonScalarPtr, uint32_t seedScalarPtr, 
                                   uint32_t vScalarPtr, uint32_t aScalarPtr, uint32_t readyFlagPtr)
{
    return AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Generate code to copy poisson input and seeds from scalar to vector memory
            AssemblerUtils::generateScalarVectorMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       poissonScalarPtr, poissonPtr, numTimesteps + 2);

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
            if(roundMode == RoundMode::STOCHASTIC) {
                ALLOCATE_SCALAR(STmp);
                c.li(*STmp, seedPtr);
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
            c.li(*SABuffer, aScalarPtr);

            // Start writing 64 bytes in (after I values)
            c.li(*SVBuffer, vScalarPtr);

            // End writing at 100 timesteps * 64 bytes
            c.li(*SVBufferEnd, vScalarPtr + (64 * numTimesteps));
    
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
        
                {
                    ALLOCATE_SCALAR(SVTmp);
                    ALLOCATE_SCALAR(SATmp);
                    for(int i = 0; i < 32; i++) {
                        c.vextract(*SVTmp, *VV, i);
                        c.vextract(*SATmp, *VA, i);

                        c.sh(*SVTmp, *SVBuffer, i * 2);
                        c.sh(*SATmp, *SABuffer, i * 2);
                    }
                }
                c.addi(*SVBuffer, *SVBuffer, 64);
                c.addi(*SABuffer, *SABuffer, 64);
    
                // While x2 (address) < x1 (count), goto loop
                c.bne(*SVBuffer, *SVBufferEnd, loop);
            }
    
        });   
}

void saveData(const volatile int16_t *outputV, const volatile int16_t *outputA,
              const std::string &outputSuffix, const std::string &filenameSuffix, size_t numTimesteps)
{
    std::ofstream out("out_alif" + outputSuffix + filenameSuffix + ".txt");
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

int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    
    std::string inputFilename = "poisson_data.bin";
    std::string outputSuffix = "";
    size_t numTimesteps = 1370;
    size_t aFixedPoint = 10;
    size_t vFixedPoint = 10;
    bool saturate = false;
    bool device = false;
    RoundMode roundMode = RoundMode::ZERO;
    
    CLI::App app{"ALIF neuron simulation"};
    app.add_option("-i,--input-filename", inputFilename, "Filename containing input data");
    app.add_option("-o,--output-suffix", outputSuffix, "Suffix to output filename");
    app.add_option("-t,--timesteps", numTimesteps, "Number of timesteps to simulate");
    app.add_option("-a,--a-fractional-bits", aFixedPoint, "Number of fractional bits to use for A state variable");
    app.add_option("-v,--v-fractional-bits", vFixedPoint, "Number of fractional bits to use for V state variable");
    app.add_flag("-s,--saturate", saturate, "Should saturating operations be used");
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");
    app.add_option("-r,--round-model", roundMode, "What round mode to use");

    CLI11_PARSE(app, argc, argv);


    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate vector memory
    const uint32_t poissonPtr = AppUtils::allocateVectorAndZero(32 * numTimesteps, vectorInitData);
    const uint32_t seedPtr = AppUtils::allocateVectorAndZero(32 * 2, vectorInitData);

    // Allocate scalar memor
    const uint32_t poissonScalarPtr = AppUtils::loadScalars(inputFilename, scalarInitData);
    const uint32_t seedScalarPtr = AppUtils::allocateScalarSeedAndInit(scalarInitData);
    const uint32_t vScalarPtr = AppUtils::allocateScalarAndZero(64 * numTimesteps, scalarInitData);
    const uint32_t aScalarPtr = AppUtils::allocateScalarAndZero(64 * numTimesteps, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Generate code
    const auto code = generateCode(!device, numTimesteps, saturate, roundMode, 20.0, 2000.0, vFixedPoint, aFixedPoint,
                                   poissonPtr, seedPtr, poissonScalarPtr, seedScalarPtr, vScalarPtr, aScalarPtr, readyFlagPtr);

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

    if(device) {
        filenameSuffix += "_device";
    }

    // Dump to coe file
    AppUtils::dumpCOE("alif" + filenameSuffix + ".coe", code);

    if(!device) {
        // Create RISC-V core with instruction and scalar data
        RISCV riscV(code, scalarInitData);
    
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
        // Run!
        riscV.run();
        
        const auto *scalarData = riscV.getScalarDataMemory().getData().data();

        saveData(reinterpret_cast<const int16_t*>(scalarData + vScalarPtr),
                 reinterpret_cast<const int16_t*>(scalarData + aScalarPtr),
                 outputSuffix, filenameSuffix, numTimesteps);
    }
    else
    {
        LOGI << "Creating device";
        Device device;
        LOGI << "Resetting";
        // Put core into reset state
        device.setEnabled(false);
        
        LOGI << "Copying instructions (" << code.size() * sizeof(uint32_t) << " bytes)";
        device.uploadCode(code);
        
        LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
        device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());
        
        LOGI << "Enabling";
        // Put core into running state
        device.setEnabled(true);
        LOGI << "Running";
        
        // Wait until ready flag
        device.waitOnNonZero(readyFlagPtr);
        LOGI << "Done";
        saveData(reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + vScalarPtr),
                 reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + aScalarPtr),
                 outputSuffix, filenameSuffix, numTimesteps);

    }
}
