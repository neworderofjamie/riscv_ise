// Standard C++ includes
#include <iostream>
#include <numeric>
#include <random>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V common includes
#include "common/CLI11.hpp"
#include "common/app_utils.h"
#include "common/device.h"
#include "common/dma_buffer.h"
#include "common/dma_controller.h"

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
void populateBuffer(int16_t *bufferData, size_t transferHalfWords)
{
    // Write vector of random data words to first half of DMA buffer
    std::random_device rng;
    std::uniform_int_distribution<int16_t> dist(std::numeric_limits<int16_t>::min(),
                                                std::numeric_limits<int16_t>::max());
    std::generate_n(bufferData, transferHalfWords,
                    [&rng, &dist]() { return dist(rng); });

    // Write -1 to second half of buffer                 
    std::fill_n(bufferData + transferHalfWords,
                transferHalfWords, -1);

    std::cout << "Data:" << std::endl;
    for (size_t i = 0; i < std::min(transferHalfWords, size_t{2048}); i++) {
        std::cout << bufferData[i] << ", ";
    }
    std::cout << std::endl;
}

void checkBuffer(const int16_t *bufferData, size_t transferHalfWords)
{
    // **TODO** move into
    if (!std::equal(bufferData, bufferData + transferHalfWords, bufferData + transferHalfWords)) {
        LOGE << "ERROR: copy incorrect";
        for (size_t i = 0; i < transferHalfWords; i++) {
            std::cout << bufferData[transferHalfWords + i] << ", ";
        }
        std::cout << std::endl;
    }
    else {
        LOGI << "Copy successful!";
    }
}
}

int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    bool device = true;
    size_t numTransfers = 10;
    size_t transferSizeHalfWords = 1024;

    CLI::App app{"DMA test FeNN example"};
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");
    app.add_option("-t,--num-transfers", numTransfers, "Number of DMA transfers to make");
    app.add_option("-s,--transfer-size", transferSizeHalfWords, "Size of each transfer (in halfwords)");

    CLI11_PARSE(app, argc, argv);

    // Check we're transferring complete
    assert((transferSizeHalfWords % 32) == 0);
    const size_t transferHalfWords = numTransfers * transferSizeHalfWords;
   
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator&, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_SCALAR(SOffsetBytes);
            ALLOCATE_SCALAR(SWriteEndBytes);
            ALLOCATE_SCALAR(STransferSizeBytes);

            Label loop;

            c.li(*SOffsetBytes, 0);
            c.li(*SWriteEndBytes, transferHalfWords * 2);
            c.li(*STransferSizeBytes, transferSizeHalfWords * 2);

            // Issue interleaved reads and writes in a loop
            c.L(loop);
            {
                ALLOCATE_SCALAR(SReadDestinationOffsetBytes);

                // Make write
                AssemblerUtils::generateDMAStartWrite(c, *SOffsetBytes, *SOffsetBytes, *STransferSizeBytes);

                // Wait for write to complete
                auto SWriteStatus = AssemblerUtils::generateDMAWaitForWriteComplete(c, scalarRegisterAllocator);

                // **THINK** what to do with this information
                
                // Calculate address at end of buffer to read data back into
                c.add(*SReadDestinationOffsetBytes, *SOffsetBytes, *SWriteEndBytes);

                // Make read
                AssemblerUtils::generateDMAStartWrite(c, *SReadDestinationOffsetBytes, *SOffsetBytes, *STransferSizeBytes);

                // Wait for read to complete
                auto SReadStatus = AssemblerUtils::generateDMAWaitForReadComplete(c, scalarRegisterAllocator);
                
                // **THINK** what to do with this information


                // Update offset and loop until we reach write end
                c.add(*SOffsetBytes, *SOffsetBytes, *STransferSizeBytes);
                c.blt(*SOffsetBytes, *SWriteEndBytes, loop);
            }
        });

    // Dump to coe file
    AppUtils::dumpCOE("dma_test_fenn.coe", code);

    
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
        LOGI << "Done";

    }
    else {
        // Create RISC-V core with instruction and scalar data
        RISCV riscV;
        riscV.setInstructions(code);
        riscV.getScalarDataMemory().setData(scalarInitData);
        
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);
        
        // Create simulated DMA controller
        DMAControllerSim dmaController(riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory());

        // Check there's enough space for 2 copies of transfers
        assert(dmaController.getDataSize() >= (2 * 2 * transferHalfWords));

        // Get pointer to simulated DMA controller buffer and populate
        int16_t *bufferData = reinterpret_cast<int16_t*>(dmaController.getData());
        populateBuffer(bufferData, transferHalfWords);
        
        // Run!
        if(!riscV.run()) {
            return 1;
        }

        // Check results
        checkBuffer(bufferData, transferHalfWords);
    }
    return 0;

}
