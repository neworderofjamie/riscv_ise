// Standard C++ includes
#include <fstream>
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

int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    bool device = true;

    CLI::App app{"Blank example"};
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");

    CLI11_PARSE(app, argc, argv);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Allocate vector arrays
    const uint32_t postAddrExc = AppUtils::allocateVectorAndZero(32, vectorInitData);
    const uint32_t postAddrInh = AppUtils::allocateVectorAndZero(32, vectorInitData);

    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_SCALAR(STargetAddrRegExc);
            ALLOCATE_SCALAR(STargetAddrRegInh);

            ALLOCATE_VECTOR(VWeightIndExc);
            ALLOCATE_VECTOR(VWeightIndInh);
            ALLOCATE_VECTOR(VPostAddrExc);
            ALLOCATE_VECTOR(VPostAddrInh);

            // Load some made up base addresses
            c.li(*STargetAddrRegExc, 64);
            c.li(*STargetAddrRegInh, 128);

            // Load some random weights
            c.vlui(*VWeightIndExc, 1324);
            c.vlui(*VWeightIndInh, (uint16_t)-16702);

            // AND.ADD
            c.vandadd(4, *VPostAddrExc, *VWeightIndExc,
                      *STargetAddrRegExc);
            c.vandadd(2, *VPostAddrInh, *VWeightIndInh,
                      *STargetAddrRegInh);

            // STORE
            c.vstore(*VPostAddrExc, Reg::X0, postAddrExc);
            c.vstore(*VPostAddrInh, Reg::X0, postAddrInh);

        });

    // Dump to coe file
    AppUtils::dumpCOE("and_add.coe", code);

    
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

    }
    else {
        // Create RISC-V core with instruction and scalar data
        RISCV riscV;
        riscV.setInstructions(code);
        riscV.getScalarDataMemory().setData(scalarInitData);
        
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);
        
        // Run!
        if(!riscV.run()) {
            return 1;
        }

        const int16_t *vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData();
        std::cout << "Exc: ";
        for(size_t i = 0; i < 32; i++) {
            std::cout << vectorData[(postAddrExc / 2) + i] << ", ";
        }
        std::cout << std::endl;
    
        std::cout << "Inh: ";
        for(size_t i = 0; i < 32; i++) {
            std::cout << vectorData[(postAddrInh / 2) + i] << ", ";
        }
        std::cout << std::endl;
    }
    return 0;

}
