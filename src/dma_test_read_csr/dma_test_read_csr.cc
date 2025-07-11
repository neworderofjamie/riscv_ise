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

    // Check there's enough space for 1024 half words
    assert(dmaBuffer.getSize() >= (2 * 1024));

    // Get halfword pointer to DMA buffer
    int16_t *bufferData = reinterpret_cast<int16_t*>(dmaBuffer.getData());
    
    // Write vector of random data words to first half of DMA buffer
    std::random_device rng;
    std::uniform_int_distribution<int16_t> dist(std::numeric_limits<int16_t>::min(),
                                                std::numeric_limits<int16_t>::max());
    std::generate_n(bufferData, 1024, 
                    [&rng, &dist](){ return dist(rng); });
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    
    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t outputPtr = AppUtils::allocateScalarAndZero(4 * 4, scalarInitData);

    // Generate code to copy 2 vectors from pointer one and 3 vectors from pointer 2 into output area
    const auto code = AssemblerUtils::generateStandardKernel(
        false, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_SCALAR(SData);
            ALLOCATE_SCALAR(SOutputBuffer);

            // Load output pointer
            c.li(*SOutputBuffer, outputPtr);

            // Read CSRs into registers and write to memory
            c.csrr(*SData, CSR::MM2S_SRC_ADDR);
            c.sw(*SData, *SOutputBuffer, 0);
            
            c.csrr(*SData, CSR::MM2S_DST_ADDR);
            c.sw(*SData, *SOutputBuffer, 4);
           
            c.csrr(*SData, CSR::MM2S_COUNT);
            c.sw(*SData, *SOutputBuffer, 8);

            c.csrr(*SData, CSR::MM2S_STATUS);
            c.sw(*SData, *SOutputBuffer, 12);
        });

    LOGI << "Creating device";
    Device device;
    LOGI << "Resetting";

    // Put core into reset state
    device.setEnabled(false);
    
    LOGI << "Copying instructions (" << code.size() * sizeof(uint32_t) << " bytes)";
    device.uploadCode(code);
    
    LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
    device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());

    // Make write
    device.getDMAController()->startWrite(0, dmaBuffer, 64, 1024 * 2);

    // Wait for write to complete
    device.getDMAController()->waitForWriteComplete();
   
    LOGI << "Enabling";
    
    // Put core into running state
    device.setEnabled(true);
    LOGI << "Running";
    
    // Wait until ready flag
    device.waitOnNonZero(readyFlagPtr);

    LOGI << "Done";

    // Get pointer to output
    const volatile uint32_t *output = reinterpret_cast<const volatile uint32_t*>(device.getDataMemory() + outputPtr);

    // **TODO** check value
    const uint32_t sourceAddress = static_cast<uint32_t>(dmaBuffer.getPhysicalAddress() & 0xFFFFFFFF);
    if(output[0] != sourceAddress) {
        LOGE << "MM2S_SRC_ADDR incorrect: " << output[0] << " vs " << sourceAddress;
    }
    
    if(output[1] != 64) {
        LOGE << "MM2S_DST_ADDR incorrect: " << output[1] << " vs " << 64;
    }
    
    if(output[2] != 1024) {
        LOGE << "MM2S_DST_ADDR incorrect: " << output[2] << " vs " << 1024;
    }
}
