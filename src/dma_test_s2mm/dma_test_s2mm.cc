// Standard C++ includes
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>

// Standard C includes
#include <cassert>

// POSIX includes
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V common includes
#include "common/app_utils.h"
#include "common/device.h"
#include "common/dma_buffer.h"
#include "common/dma_controller.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

int main()
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Create DMA buffer
    DMABuffer dmaBuffer;

    // Check there's enough space for 5 vectors
    assert(dmaBuffer.getSize() > (32 * 2 * 5));

    // Get halfword pointer to DMA buffer
    volatile int16_t *bufferData = reinterpret_cast<volatile int16_t*>(dmaBuffer.getData());

    // Write -1 to destination buffer                 
    std::fill_n(bufferData + 64, 64, -1);

    // Create memory contents
    /*std::vector<uint8_t> scalarInitData;
    
    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t outputPtr = AppUtils::allocateScalarAndZero(64 * 5, scalarInitData);*/

    const uint32_t vectorTwoPtr = 400 * 1024;

    // Generate code to copy 2 vectors from pointer one and 3 vectors from pointer 2 into output area
    /*const auto code = AssemblerUtils::generateStandardKernel(
        false, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            AssemblerUtils::generateVectorScalarMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       vectorOnePtr, outputPtr, 2);
            AssemblerUtils::generateVectorScalarMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       vectorTwoPtr, outputPtr + (64 * 2), 3);                                                       
        });

    LOGI << "Creating device"
    Device device;
    LOGI << "Resetting";

    // Put core into reset state
    device.setEnabled(false);
    
    LOGI << "Copying instructions (" << code.size() * sizeof(uint32_t) << " bytes)";
    device.uploadCode(code);
    
    LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
    device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());*/
    
    // Open memory
    // **NOTE** O_SYNC turns of caching
    int memory = open("/dev/mem", O_RDWR | O_SYNC);
    if(memory == -1) {
        throw std::runtime_error("/dev/mem open failure (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Create DMA controller
    DMAController dmaController(memory, 0xA0000000);

    // Read 64 elements back from vectorTwoPtr to DMA buffer
    dmaController.startRead(dmaBuffer, 64 * 2, vectorTwoPtr, 64 * 2);

    // Wait for read to complete
    dmaController.waitForReadComplete();
    
    for(size_t i = 64; i < 128; i++) {
        std::cout << bufferData[i] << ", ";
    }
    std::cout << std::endl;
    
    close(memory);
    /*LOGI << "Enabling";
    // Put core into running state
    device.setEnabled(true);
    LOGI << "Running";
    
    // Wait until ready flag
    device.waitOnNonZero(readyFlagPtr);

    LOGI << "Done";

    // Get pointer to output
    const volatile int16_t *output = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + outputPtr);

    if(!std::equal(bufferData, bufferData + (32 * 5), output)) {
        LOGE << "ERROR: copy incorrect";
        for(int i = 0; i < (32 * 5); i++) {
            LOGI << i << ":" << output[i] << " vs " << bufferData[i];
        }
    }*/
}
