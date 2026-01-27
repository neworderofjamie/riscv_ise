// Standard C++ includes
#include <fstream>
#include <random>

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


std::vector<uint32_t> generateCode(bool simulate, uint32_t numTimesteps, uint32_t vectorSeedPtr, uint32_t vectorRecordingPtr,
                                   uint32_t scalarSeedPtr, uint32_t scalarRecordingPtr, uint32_t readyFlagPtr)
{
    return AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Copy seed from scalar to vector memory
            AssemblerUtils::generateScalarVectorMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       scalarSeedPtr, vectorSeedPtr, 2u);
            // Register allocation
            ALLOCATE_SCALAR(SIBuffer);
            ALLOCATE_SCALAR(SIBufferEnd);
            ALLOCATE_VECTOR(VExpMinusLambda);
            ALLOCATE_VECTOR(VI);
            ALLOCATE_VECTOR(VOne);

            // Labels
            auto timeLoop = createLabel();
            auto poissonLoop = createLabel();
            auto poissonStart = createLabel();
            auto poissonEnd = createLabel();

            // Load RNG seed from first 128 bytes of vector memory
            {
                ALLOCATE_SCALAR(STmp);
                c.li(*STmp, vectorSeedPtr);
                c.vloadr0(*STmp);
                c.vloadr1(*STmp, 64);
            }

            // Load immediates
            c.vlui(*VExpMinusLambda, convertFixedPoint(std::exp(-5.0), 14));
            c.vlui(*VOne, 1);

            // Start writing at start
            c.li(*SIBuffer, vectorRecordingPtr);

            // End writing after 64 bytes for each timestep
            c.li(*SIBufferEnd, vectorRecordingPtr + (numTimesteps * 64));

            // Loop over time
            c.L(timeLoop);
            {
                ALLOCATE_SCALAR(SMask);
                ALLOCATE_VECTOR(VNumSpikes);
                ALLOCATE_VECTOR(VP);

                c.L(poissonStart);
                c.vlui(*VNumSpikes, 0);
                c.vlui(*VP, convertFixedPoint(1.0, 14));
                c.li(*SMask, 0xFFFFFFFF);
                c.L(poissonLoop);
                {
                    ALLOCATE_VECTOR(VNewNumSpikes);
                    ALLOCATE_SCALAR(SNewMask);
                    ALLOCATE_VECTOR(VRand);

                    // Generate uniformly distributed random number
                    c.vrng(*VRand);

                    // NumSpikes++
                    c.vadd(*VNewNumSpikes, *VNumSpikes, *VOne);

                    // P *= VRand
                    c.vmul(15, *VP, *VP, *VRand);

                    //SNewMask = ExpMinusLambda < p
                    c.vtlt(*SNewMask, *VExpMinusLambda, *VP);

                    // VNumSpikes = SMask ? VNewNumSpikes : VNumSpikes
                    c.vsel(*VNumSpikes, *SMask, *VNewNumSpikes);

                    // VNumSpikes = SMask ? VNewNumSpikes : VNumSpikes
                    c.and_(*SMask, *SMask, *SNewMask);

                    c.bne(*SMask, Reg::X0, poissonLoop);
                }
        
                c.vsub(*VI, *VNumSpikes, *VOne);
                c.L(poissonEnd);
                //vmem[a...a+32] = v
                c.vstore(*VI, *SIBuffer);
                c.addi(*SIBuffer, *SIBuffer, 64);
    
                // While x2 (address) < x1 (count), goto loop
                c.bne(*SIBuffer, *SIBufferEnd, timeLoop);
            }

            // Copy recording data from vector to scalar memory
            AssemblerUtils::generateVectorScalarMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       vectorRecordingPtr, scalarRecordingPtr, numTimesteps);

            LOGI << "Poisson start:" << poissonStart.getAddress();
            LOGI << "Poisson end:" << poissonEnd.getAddress();
        });
}


int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    bool device = false;
    bool dumpCoe = false;
    uint32_t numTimesteps = 100;

    CLI::App app{"Poisson generator"};
    app.add_option("-n,--num-timesteps", numTimesteps, "How many timesteps to simulate for");
    app.add_flag("-c,--dump-coe", dumpCoe, "Should a .coe file for simulation in the Xilinx simulator be dumped");
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");

    CLI11_PARSE(app, argc, argv);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;
    
    // Allocate vector arrays
    // **NOTE** these are adjacent so data can be block-copied from scalar memory
    const uint32_t vectorSeedPtr = AppUtils::allocateVectorAndZero(64, vectorInitData);
    const uint32_t vectorRecordingPtr = AppUtils::allocateVectorAndZero(32 * numTimesteps, vectorInitData);
   
    // Allocate scalar arrays
    const uint32_t scalarSeedPtr = AppUtils::allocateScalarSeedAndInit(scalarInitData);
    const uint32_t scalarRecordingPtr = AppUtils::allocateScalarAndZero(64 * numTimesteps, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
        
    const auto code = generateCode(!device, numTimesteps, vectorSeedPtr, vectorRecordingPtr,
                                   scalarSeedPtr, scalarRecordingPtr, readyFlagPtr);

    if(dumpCoe) {
        AppUtils::dumpCOE("poisson.coe", code);

        std::vector<uint32_t> wordData(scalarInitData.size() / 4);
        std::memcpy(wordData.data(), scalarInitData.data(), scalarInitData.size());
        AppUtils::dumpCOE("poisson_data.coe", wordData);
    }

    if(device) {
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
        device.setEnabled(false);
        LOGI << "Done";

        const volatile int16_t *scalarRecordingData = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + scalarRecordingPtr);
    
        std::ofstream out("out_poisson_device.txt");
        for(size_t t = 0; t < 32 * numTimesteps; t++) {
            out << *scalarRecordingData++ << std::endl;
        }
    }
    else {
        // Build ISE with vector co-processor
        RISCV riscV;
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);

        // Set instructions and vector init data
        riscV.setInstructions(code);
        riscV.getScalarDataMemory().setData(scalarInitData);
    
        // Run!
        riscV.run();
    
        auto *scalarData = riscV.getScalarDataMemory().getData();
        const int16_t *scalarRecordingData = reinterpret_cast<const int16_t*>(scalarData + scalarRecordingPtr);
    
        std::ofstream out("out_poisson.txt");
        for(size_t t = 0; t < 32 * numTimesteps; t++) {
            out << *scalarRecordingData++ << std::endl;
        }

        std::ofstream heatmapFile("poisson_heatmap.txt");
        for(size_t i = 0; i < riscV.getInstructionHeatmap().size(); i++) {
            heatmapFile << (code.at(i) & 0b1111111) << ", " << riscV.getInstructionHeatmap()[i] << std::endl;
        }
    }
    
}
