// Standard C++ includes
#include <algorithm>
#include <numeric>

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
    int16_t *bufferData = reinterpret_cast<int16_t*>(dmaBuffer.getData());
    
    // Write five vectors of half words to DMA buffer
    std::iota(bufferData, bufferData + (32 * 5), 0);

    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    
    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t outputPtr = AppUtils::allocateScalarAndZero(64 * 5, scalarInitData);

    const uint32_t vectorOnePtr = 0;
    const uint32_t vectorTwoPtr = 400 * 1024;

    // Generate code to copy 2 vectors from pointer one and 3 vectors from pointer 2 into output area
    const auto code = AssemblerUtils::generateStandardKernel(
        false, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            AssemblerUtils::generateVectorScalarMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       vectorOnePtr, outputPtr, 2);
            AssemblerUtils::generateVectorScalarMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       vectorTwoPtr, outputPtr + (64 * 2), 3);                                                       
        });

    LOGI << "Creating device";
    Device device;
    LOGI << "Resetting";

    // Put core into reset state
    device.setEnabled(false);
    
    // Write 2 vectors from DMA buffer to vectorOnePtr
    device.getDMAController()->startWrite(vectorOnePtr, dmaBuffer, 0, 64 * 2);

    // Wait for write to complete
    device.getDMAController()->waitForWriteComplete();

    // Write 3 vectors from DMA buffer to URAM address 
    device.getDMAController()->startWrite(vectorTwoPtr, dmaBuffer, 64 * 2, 64 * 3);

    // Wait for write to complete
    device.getDMAController()->waitForWriteComplete();

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

    // Get pointer to output
    const volatile int16_t *output = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + outputPtr);

    if(!std::equal(bufferData, bufferData + (32 * 5), output)) {
        LOGE << "ERROR: copy incorrect";
        for(int i = 0; i < (32 * 5); i++) {
            LOGI << output[i] << " vs " << bufferData[i];
        }
    }
}
