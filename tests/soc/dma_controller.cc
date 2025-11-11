// Standard C++ includes
#include <fstream>
#include <numeric>
#include <random>
#include <tuple>

// PLOG includes
#include <plog/Severity.h>

// Google test includes
#include "gtest/gtest.h"

// RISC-V common includes
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

// SoC test includes
#include "fixtures.h"

// Anonymous namespace
namespace
{
//----------------------------------------------------------------------------
// TransferSizeTest
//----------------------------------------------------------------------------
class TransferSizeTest : public testing::TestWithParam<std::tuple<size_t, size_t>> 
{
};

//----------------------------------------------------------------------------
// TransferSizeDeviceTest
//----------------------------------------------------------------------------
class TransferSizeDeviceTest : public testing::TestWithParam<std::tuple<size_t, size_t, bool>> 
{
};

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

bool checkBuffer(const int16_t *bufferData, size_t transferHalfWords)
{
    // **TODO** move into
    if (!std::equal(bufferData, bufferData + transferHalfWords, bufferData + transferHalfWords)) {
        LOGE << "ERROR: copy incorrect";
        for (size_t i = 0; i < transferHalfWords; i++) {
            std::cout << bufferData[transferHalfWords + i] << ", ";
        }
        std::cout << std::endl;
        return false;
    }
    else {
        LOGI << "Copy successful!";
        return true;
    }
}
}

//--------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------
TEST_P(TransferSizeTest, HostDMA)
{
    const size_t transferHalfWords = std::get<0>(GetParam()) * std::get<1>(GetParam());

    // Create DMA buffer
    DMABuffer dmaBuffer;

    // Check there's enough space for 2 copies of transfers
    ASSERT_GE(dmaBuffer.getSize(), (2 * 2 * transferHalfWords));

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
    for(size_t offsetBytes = 0; offsetBytes < (2 * transferHalfWords); offsetBytes+=(std::get<1>(GetParam()) * 2)) {
        // Make write
        dmaController.startWrite(offsetBytes, dmaBuffer, offsetBytes, std::get<1>(GetParam()) * 2);

        // Wait for write to complete
        dmaController.waitForWriteComplete();
        
        // Make read
        dmaController.startRead(dmaBuffer, (transferHalfWords * 2) + offsetBytes, offsetBytes, std::get<1>(GetParam()) * 2);

        // Wait for read to complete
        dmaController.waitForReadComplete();
    }
    
    if(!std::equal(bufferData, bufferData + transferHalfWords, bufferData + transferHalfWords)) {
        LOGE << "ERROR: copy incorrect";
        for(size_t i = 0; i < transferHalfWords; i++) {
            std::cout << bufferData[transferHalfWords + i] << ", ";
        }
        std::cout << std::endl;
        FAIL();
    }
    else {
        LOGI << "Copy successful!";
    }
    close(memory);
}

TEST(DMAController, ReadCSR)
{
    // Create DMA buffer
    DMABuffer dmaBuffer;

    // Check there's enough space for 1024 half words
    ASSERT_GE(dmaBuffer.getSize(), (2 * 1024));

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
    device.getDMAController()->startWrite(64, dmaBuffer, 0, 1024 * 2);

    // Wait for write to complete
    device.getDMAController()->waitForWriteComplete();
   
    LOGI << "Enabling";
    
    // Put core into running state
    device.setEnabled(true);
    LOGI << "Running";
    
    // Wait until ready flag
    device.waitOnNonZero(readyFlagPtr);
    device.setEnabled(false);

    LOGI << "Done";

    // Get pointer to output
    const volatile uint32_t *output = reinterpret_cast<const volatile uint32_t*>(device.getDataMemory() + outputPtr);

    const uint32_t sourceAddress = static_cast<uint32_t>(dmaBuffer.getPhysicalAddress() & 0xFFFFFFFF);
    ASSERT_EQ(output[0], sourceAddress);
    ASSERT_EQ(output[1], 64);
    ASSERT_EQ(output[2], 2048);
    LOGI << "MM2S_STATUS:" << output[3];
}

TEST_P(TransferSizeDeviceTest, FeNNDMA)
{
    // Check we're transferring complete
    assert((std::get<1>(GetParam()) % 32) == 0);
    const size_t transferHalfWords = std::get<0>(GetParam()) * std::get<1>(GetParam());
   
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t baseAddressPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !std::get<2>(GetParam()), readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator&, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_SCALAR(SOffsetBytes);
            ALLOCATE_SCALAR(SWriteEndBytes);
            ALLOCATE_SCALAR(STransferSizeBytes);
            ALLOCATE_SCALAR(SBaseAddress);

            Label loop;

            c.li(*SOffsetBytes, 0);
            c.li(*SWriteEndBytes, transferHalfWords * 2);
            c.li(*STransferSizeBytes, std::get<1>(GetParam()) * 2);
            
            // Load base address
            c.li(*SBaseAddress, baseAddressPtr);
            c.lw(*SBaseAddress, *SBaseAddress);

            // Issue interleaved reads and writes in a loop
            c.L(loop);
            {
                ALLOCATE_SCALAR(SDRAMAddress);

                // Calculate read address in SDRAM
                c.add(*SDRAMAddress, *SBaseAddress, *SOffsetBytes);

                // Make write
                AssemblerUtils::generateDMAStartWrite(c, *SOffsetBytes, *SDRAMAddress, *STransferSizeBytes);

                // **YUCK** MM2S_STATUS write takes a cycle to propagate
                c.nop();

                // Wait for write to complete
                auto SWriteStatus = AssemblerUtils::generateDMAWaitForWriteComplete(c, scalarRegisterAllocator);

                // **THINK** what to do with this information
                
                // Calculate address at end of buffer to read data back into
                c.add(*SDRAMAddress, *SDRAMAddress, *SWriteEndBytes);

                // Make read
                AssemblerUtils::generateDMAStartRead(c, *SDRAMAddress, *SOffsetBytes, *STransferSizeBytes);

                // **YUCK** S2MM_STATUS write takes a cycle to propagate
                c.nop();

                // Wait for read to complete
                auto SReadStatus = AssemblerUtils::generateDMAWaitForReadComplete(c, scalarRegisterAllocator);
                
                // **THINK** what to do with this information


                // Update offset and loop until we reach write end
                c.add(*SOffsetBytes, *SOffsetBytes, *STransferSizeBytes);
                c.blt(*SOffsetBytes, *SWriteEndBytes, loop);
            }
        });


    if(std::get<2>(GetParam())) {
        LOGI << "Creating device";
        Device device;

        // Create DMA buffer
        DMABuffer dmaBuffer;

        // Check there's enough space for 2 copies of transfers
        ASSERT_GE(dmaBuffer.getSize(), (2 * 2 * transferHalfWords));

        // Write physical address of DMA buffer to scalar init
        uint32_t *scalarInitWords = reinterpret_cast<uint32_t*>(scalarInitData.data());
        scalarInitWords[baseAddressPtr / 4] = dmaBuffer.getPhysicalAddress();

        // Get pointer to simulated DMA controller buffer and populate
        int16_t *bufferData = reinterpret_cast<int16_t*>(dmaBuffer.getData());
        populateBuffer(bufferData, transferHalfWords);
        
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

        // Check results
        checkBuffer(bufferData, transferHalfWords);
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
        riscV.setDMAController(&dmaController);

        // Check there's enough space for 2 copies of transfers
        ASSERT_GE(dmaController.getDataSize(), (2 * 2 * transferHalfWords));

        // Get pointer to simulated DMA controller buffer and populate
        int16_t *bufferData = reinterpret_cast<int16_t*>(dmaController.getData());
        populateBuffer(bufferData, transferHalfWords);
        
        // Run!
        if(!riscV.run()) {
            FAIL();
        }

        // Check results
        checkBuffer(bufferData, transferHalfWords);
    }
}


INSTANTIATE_TEST_SUITE_P(DMAController,
                         TransferSizeTest,
                         testing::Values(std::make_tuple(10, 1024), std::make_tuple(1, 128 * 1024)));

INSTANTIATE_TEST_SUITE_P(DMAController,
                         TransferSizeDeviceTest,
                         testing::Values(std::make_tuple(10, 1024, true), std::make_tuple(1, 128 * 1024, true),
                                         std::make_tuple(10, 1024, false), std::make_tuple(1, 128 * 1024, false)));
