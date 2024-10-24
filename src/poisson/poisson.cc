// Standard C++ includes
#include <fstream>
#include <random>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V common includes
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
                                                       scalarSeedPtr, vectorSeedPtr, 2);
            // Register allocation
            ALLOCATE_SCALAR(SIBuffer);
            ALLOCATE_SCALAR(SIBufferEnd);
            ALLOCATE_VECTOR(VExpMinusLambda);
            ALLOCATE_VECTOR(VI);

            // Labels
            Label timeLoop;
            Label poissonLoop;

            // Load RNG seed from first 128 bytes of vector memory
            {
                ALLOCATE_SCALAR(STmp);
                c.li(*STmp, vectorSeedPtr);
                c.vloadr0(*STmp);
                c.vloadr1(*STmp, 64);
            }

            c.vlui(*VExpMinusLambda, convertFixedPoint(std::exp(-20.0 * 256 / 1000.0), 14));

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

            // Copy recording data from vector to scalar memory
            AssemblerUtils::generateVectorScalarMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       vectorRecordingPtr, scalarRecordingPtr, numTimesteps);
        });
}


int main()
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    constexpr uint32_t numTimesteps = 100;
    constexpr bool simulate = true;
    constexpr bool dump = true;

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
        
    const auto code = generateCode(simulate, numTimesteps, vectorSeedPtr, vectorRecordingPtr,
                                   scalarSeedPtr, scalarRecordingPtr, readyFlagPtr);

    if(dump) {
        AppUtils::dumpCOE("poisson.coe", code);

        std::vector<uint32_t> wordData(scalarInitData.size() / 4);
        std::memcpy(wordData.data(), scalarInitData.data(), scalarInitData.size());
        AppUtils::dumpCOE("poisson_data.coe", wordData);
    }
    if(simulate) {
        // Create RISC-V core with instruction and scalar data
        RISCV riscV(code, scalarInitData);
    
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
        // Run!
        riscV.run();
    
        auto *scalarData = riscV.getScalarDataMemory().getData().data();
        const int16_t *scalarRecordingData = reinterpret_cast<const int16_t*>(scalarData + scalarRecordingPtr);
    
        std::ofstream out("out_poisson.txt");
        for(size_t t = 0; t < 32 * numTimesteps; t++) {
            out << *scalarRecordingData++ << std::endl;
        }
    }
    else {
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

        const volatile int16_t *scalarRecordingData = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + scalarRecordingPtr);
    
        std::ofstream out("out_poisson_device.txt");
        for(size_t t = 0; t < 32 * numTimesteps; t++) {
            out << *scalarRecordingData++ << std::endl;
        }
    }
}
