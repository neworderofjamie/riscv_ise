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
    constexpr uint32_t numTimesteps = 100;
    constexpr bool simulate = false;

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t outputPtr = AppUtils::allocateScalarAndZero(64, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    
    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
           ALLOCATE_VECTOR(VA);
           ALLOCATE_VECTOR(VB);
           ALLOCATE_VECTOR(VC);
           ALLOCATE_VECTOR(VZero);        
   
           c.vlui(*VA, 38);
           c.vlui(*VB, convertFixedPoint(std::exp(-1.0 / 20.0), 15));
           c.vlui(*VZero, 0);
           c.vmul(15, *VC, *VA, *VB);
           c.vadd(*VC, *VC, *VZero);
           {
               ALLOCATE_SCALAR(STmp);
               ALLOCATE_SCALAR(SAddress);
               c.li(*SAddress, outputPtr);
               for(int l = 0; l < 32; l++) {
                   c.vextract(*STmp, *VC, l);
                   c.sh(*STmp, *SAddress, l * 2);
               }
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
        for(size_t i = 0; i < 32; i++) {
            std::cout << *output++ << ", ";
        }
        std::cout << std::endl;

    
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
        for(size_t i = 0; i < 32; i++) {
            std::cout << *output++ << ", ";
        }
        std::cout << std::endl;


    }
   
    

}
