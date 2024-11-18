// Standard C++ includes
#include <fstream>
#include <numeric>

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

int main()
{
    constexpr uint32_t numHidden = 32;
    constexpr uint32_t numTimesteps = 10;
    constexpr bool simulate = false;
    constexpr uint32_t numHiddenSpikeWords = ceilDivide(numHidden, 32);

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t outputPtr = AppUtils::allocateScalarAndZero(numHiddenSpikeWords * numTimesteps * 64, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    
    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Register allocation
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeEnd);
            ALLOCATE_SCALAR(SHiddenVarRecordingBuffer);
            ALLOCATE_VECTOR(VV);
            // Labels
            Label timeLoop;
            Label spinLoop;

            // Set timestep range and load ready flag pointer
            c.li(*STime, 0);
            c.li(*STimeEnd, numTimesteps);
            c.li(*SHiddenVarRecordingBuffer, outputPtr);

            c.vlui(*VV, 0);
          
            // Loop over time
            c.L(timeLoop);
            {

                // ---------------------------------------------------------------
                // Hidden neurons
                // ---------------------------------------------------------------
                {
                    // Register allocation
                    ALLOCATE_VECTOR(VAlpha);

                    // Load constants
                    // alpha = e^(-1/20)
                    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / 20.0), 5));

                    {
                        // Register allocation
                        ALLOCATE_VECTOR(VISyn);
          
                        c.vlui(*VISyn, 26);

                        // VV *= VAlpha
                        c.vmul(5, *VV, *VV, *VAlpha);
                        
                        // VV += VISyn
                        c.vadd(*VV, *VV, *VISyn);

                        ALLOCATE_SCALAR(STmp);
                        for(int l = 0; l < 32; l++) {
                            c.vextract(*STmp, *VV, l);
                            c.nop();
                            c.sh(*STmp, *SHiddenVarRecordingBuffer, l * 2);
                        }
                        c.addi(*SHiddenVarRecordingBuffer, *SHiddenVarRecordingBuffer, 64);
                 
                    }  
                }

                c.addi(*STime, *STime, 1);
                c.bne(*STime, *STimeEnd, timeLoop);
            }
        });

    // Dump to coe file
    AppUtils::dumpCOE("blank_bad_forward.coe", code);

    if(simulate) {
        // Create RISC-V core with instruction and scalar data
        RISCV riscV(code, scalarInitData);
        
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
        
        // Run!
        if(!riscV.run()) {
            return 1;
        }
        
        auto *scalarData = riscV.getScalarDataMemory().getData().data();
        const int16_t *output = reinterpret_cast<const int16_t*>(scalarData + outputPtr);
        for(size_t i = 0; i < numTimesteps; i++) {
            for(size_t i = 0; i < numHidden; i++) {
                std::cout << *output++ << ", ";
            }
            std::cout << std::endl;
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
        
        const volatile int16_t *output = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + outputPtr);
        for(size_t i = 0; i < numTimesteps; i++) {
            for(size_t i = 0; i < numHidden; i++) {
                std::cout << *output++ << ", ";
            }
            std::cout << std::endl;
        }


    }
   
    

}
