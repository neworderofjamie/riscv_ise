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
#include "common/dma_buffer.h"
#include "common/dma_controller.h"


int main()
{
    constexpr size_t numTransfers = 1;
    constexpr size_t transferSizeHalfWords = 128;
    constexpr size_t transferHalfWords = numTransfers * transferSizeHalfWords;

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Create DMA buffer
    DMABuffer dmaBuffer;

    // Check there's enough space for 2 copies of transfers
    assert(dmaBuffer.getSize() >= (2 * 2 * transferHalfWords));

    // Get halfword pointer to DMA buffer
    int16_t *bufferData = reinterpret_cast<int16_t*>(dmaBuffer.getData());
    
    // Write vector of random data words to first half of DMA buffer
    std::random_device rng;
    std::uniform_int_distribution<int16_t> dist(std::numeric_limits<int16_t>::min(),
                                                std::numeric_limits<int16_t>::max());
    std::generate_n(bufferData, transferHalfWords, 
                    [&rng, &dist](){ return dist(rng); });
    
    // Write -1 to second half of buffer                 
    std::fill_n(bufferData + transferHalfWords, 
                transferHalfWords, -1);
    
    std::cout << "Data:" << std::endl;
    for(size_t i = 0; i < std::min(transferHalfWords, size_t{2048}); i++) {
        std::cout << bufferData[i] << ", ";
    }
    std::cout << std::endl;
                    
  
    // Open memory
    // **NOTE** O_SYNC turns of caching
    int memory = open("/dev/mem", O_RDWR | O_SYNC);
    if(memory == -1) {
        throw std::runtime_error("/dev/mem open failure (" + std::to_string(errno) + " = " + strerror(errno) + ")");
    }

    // Create DMA controller
    DMAController dmaController(memory, 0x80300000);
    
    // Issue interleaved reads and writes
    for(size_t offsetBytes = 0; offsetBytes < (2 * transferHalfWords); offsetBytes+=(transferSizeHalfWords * 2)) {
        // Make write
        dmaController.startWrite(offsetBytes, dmaBuffer, offsetBytes, transferSizeHalfWords * 2);

        // Wait for write to complete
        dmaController.waitForWriteComplete();
        
        // Make read
        dmaController.startRead(dmaBuffer, (transferHalfWords * 2) + offsetBytes, offsetBytes, transferSizeHalfWords * 2);

        // Wait for read to complete
        dmaController.waitForReadComplete();
    }
    
    if(!std::equal(bufferData, bufferData + transferHalfWords, bufferData + transferHalfWords)) {
        LOGE << "ERROR: copy incorrect";
        for(size_t i = 0; i < transferHalfWords; i++) {
            std::cout << bufferData[transferHalfWords + i] << ", ";
        }
        std::cout << std::endl;
    }
    else {
        LOGI << "Copy successful!";
    }
    close(memory);
}
