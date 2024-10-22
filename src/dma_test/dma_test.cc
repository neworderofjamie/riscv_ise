// Standard C++ includes
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
#include "common/dma_buffer.h"
#include "common/dma_controller.h"


int main()
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Create DMA buffer
    DMABuffer dmaBuffer;

    // Check there's enough space for 4 vectors
    assert(dmaBuffer.getSize() > (32 * 2 * 4));

    // Get halfword pointer to DMA buffer
    int16_t *bufferData = reinterpret_cast<int16_t*>(dmaBuffer.getData());
    
    // Write one vector of half words to DMA buffer
    std::iota(bufferData, bufferData + 32, 0);

    // Open memory
    // **NOTE** O_SYNC turns of caching
    int memory = open("/dev/mem", O_RDWR | O_SYNC);
    if(memory == -1) {
        throw std::runtime_error("/dev/mem open failure (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Create DMA controller
    DMAController dmaController(memory, 0xA0000000, 0xA0010000);

    // Write vector from buffer
    dmaController.startWrite(0, dmaBuffer, 0, 32 * 2);

    // Wait for write to complete
    dmaController.waitForWriteComplete();
}
