// Standard C++ includes
#include <fstream>
#include <iostream>
#include <numeric>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V common includes
#include "common/CLI11.hpp"
#include "common/app_utils.h"
#include "common/device.h"
#include "common/device_control.h"
#include "common/dma_buffer.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/dma_controller_sim.h"
#include "ise/riscv.h"
#include "ise/vector_processor.h"

namespace
{
void printVector(const int16_t *v)
{
    for(int i = 0; i < 32; i++) {
        std::cout << *v++ << ", ";
    }
    std::cout << std::endl;
}
}

int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    bool device = false;

    CLI::App app{"Blank example"};
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");

    CLI11_PARSE(app, argc, argv);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    
    // Allocate vector arrays
    const uint32_t inputPtr = AppUtils::allocateVectorAndZero(32 * 2, vectorInitData);
    const uint32_t outputPtr = AppUtils::allocateVectorAndZero(32, vectorInitData);

    // Write test vector to vector memory
    std::iota(vectorInitData.begin() + (inputPtr / 2),
              vectorInitData.begin() + (inputPtr / 2) + (32 * 2), 0);
    
    std::cout << "A:";
    printVector(vectorInitData.data() + (inputPtr / 2));
    
    std::cout << "B:";
    printVector(vectorInitData.data() + (inputPtr / 2) + 32);
    
    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_VECTOR(VA);
            ALLOCATE_VECTOR(VB);
            ALLOCATE_VECTOR(VOutput);
         
            // Load vectors from memory
            c.vloadv(*VA, Reg::X0, inputPtr);
            c.vloadv(*VB, Reg::X0, inputPtr + 64);
            
            // **STALL**
            c.nop();
            
            // Perform operation
            c.vadd(*VOutput, *VA, *VB);
            
            // Store result
            c.vstore(*VOutput, Reg::X0, outputPtr);
        });

    // Dump to coe file
    AppUtils::dumpCOE("blank_math.coe", code);

    
    if(device) {
        LOGI << "Creating device";
        Device device;
        DeviceControl deviceControl(1);
        LOGI << "Resetting";
        // Put core into reset state
        deviceControl.setEnabled(false);
        
        // Create DMA buffer
        DMABuffer dmaBuffer;
        
        // Check there's enough space for vector init data
        assert(dmaBuffer.getSize() > (vectorInitData.size() * 2));
        
        // Get halfword pointer to DMA buffer
        int16_t *bufferData = reinterpret_cast<int16_t*>(dmaBuffer.getData());
        
        // Check there's enough space for vector init data
        assert(dmaBuffer.getSize() > (32 * 2));

        {
            LOGI << "DMAing A and B vectors to device";
           
            // Copy vector init data to buffer
            std::copy(vectorInitData.cbegin(), vectorInitData.cend(), bufferData);
            
            // Start DMA of data to URAM
            device.getDMAController()->startWrite(0, dmaBuffer, 0, vectorInitData.size() * 2);
    
            // Wait for write to complete
            device.getDMAController()->waitForWriteComplete();
        }
        
        LOGI << "Copying instructions (" << code.size() * sizeof(uint32_t) << " bytes)";
        device.uploadCode(code);
        
        LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
        device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());
        
        LOGI << "Enabling";
        // Put core into running state
        deviceControl.setEnabled(true);
        LOGI << "Running";
        
        // Wait until ready flag
        device.waitOnNonZero(readyFlagPtr);
        deviceControl.setEnabled(false);
        
        {
            LOGI << "DMAing output vector off device";
           
            // Start DMA of data to DRAM
            device.getDMAController()->startRead(dmaBuffer, 0, 0, vectorInitData.size() * 2);
    
            // Wait for read to complete
            device.getDMAController()->waitForReadComplete();
            
            std::cout << "Output: ";
            printVector(bufferData + (outputPtr / 2));
        }

    }
    else {
        // Create RISC-V core with instruction and scalar data
        RISCV riscV;
        riscV.setInstructions(code);
        riscV.getScalarDataMemory().setData(scalarInitData);
        
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);
        riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().setData(vectorInitData);
        
        // Run!
        if(!riscV.run()) {
            return 1;
        }
        
        std::cout << "Output: ";
        printVector(riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData() + (outputPtr / 2));
    
    }
    return 0;

}
