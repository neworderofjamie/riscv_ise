// Standard C++ includes
#include <fstream>
#include <numeric>

// Standard C includes
#include <cassert>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V common includes
#include "common/CLI11.hpp"
#include "common/app_utils.h"
#include "common/device.h"
#include "common/utils.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/router_sim.h"
#include "ise/shared_bus_sim.h"
#include "ise/vector_processor.h"

namespace
{
void checkOutput(const volatile uint32_t *wordData, uint32_t outputSpikeArrayEnd, uint32_t spikeArrayPtr,
                 uint32_t outputSpikeArrayPtr)
{
    const uint32_t num = (wordData[outputSpikeArrayEnd / 4] - spikeArrayPtr) / 4;

    // Loop through events received by one core
    uint32_t bitfield = 0xDEADBEEF;
    std::cout << num << " spikes received (" << popCount(bitfield) << " expected)" << std::endl;
    for(uint32_t i = 0; i < num; i++) {
        const uint32_t spike = wordData[i + (outputSpikeArrayPtr / 4)];
        std::cout << std::hex << "\t" << spike << std::endl;

        // Split event into core and neuron
        const uint32_t base = spike >> 5;
        const uint32_t neuron = spike & ((1 << 5) - 1);
        const uint32_t neuronBit = 1 << neuron;
        
        // Check base is correct and bit is still set in core's bitmask
        assert(base == 0x37AB);
        assert(bitfield & neuronBit);

        // Clear BIT
        bitfield &= ~neuronBit;
    }

    // Check all bits have been zeroed
    assert(bitfield == 0);
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
    const uint32_t outputSpikeArrayEnd = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t outputSpikeArrayPtr = AppUtils::allocateScalarAndZero(32 * 4, scalarInitData);

    const uint32_t spikeArrayPtr = 32 * 4096;

    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator&, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            
            // Build 0x1F000 immediate (address of start of spike memory)
            ALLOCATE_SCALAR(SSpikeMemory);
            c.li(*SSpikeMemory, spikeArrayPtr);
            
            // Write SLAVE_EVENT_ADDRESS
            c.csrw(CSR::SLAVE_EVENT_ADDRESS, *SSpikeMemory);

            {
                // Build 0x6F560 immediate (random master event ID base)
                ALLOCATE_SCALAR(STmp);
                c.li(*STmp, 0x6F560);

                // Write MASTER_EVENT_ID_BASE
                c.csrw(CSR::MASTER_EVENT_ID_BASE, *STmp);
            }
            
            
            {
                // Build 0xDEADBEEF immediate (random spike bitmask)
                ALLOCATE_SCALAR(STmp);
                c.li(*STmp, 0xDEADBEEF);
            
                // Write MASTER_EVENT_BITFIELD
                c.csrw(CSR::MASTER_EVENT_BITFIELD, *STmp);
            }

            // Wait to give the spikes time to be processed
            for(int i = 0; i < 100; i++) {
                c.nop();
            }
            
            {
                // Read SLAVE_EVENT_ADDRESS i.e. where slave FINISHED writing spikes
                ALLOCATE_SCALAR(SSpikeMemoryEnd);
                c.csrr(*SSpikeMemoryEnd, CSR::SLAVE_EVENT_ADDRESS);
                
                // Write to memory
                c.sw(*SSpikeMemoryEnd, Reg::X0, outputSpikeArrayEnd);

                // Load address of output array in normal memory
                ALLOCATE_SCALAR(SSpikeOut);
                c.li(*SSpikeOut, outputSpikeArrayPtr);

                auto spikeLoop = c.L();
                {
                    // Load word from spike memory and store in data memory
                    ALLOCATE_SCALAR(STmp);
                    c.lw(*STmp, *SSpikeMemory);
                    c.sw(*STmp, *SSpikeOut);

                    // Loop until all spikes processed
                    c.addi(*SSpikeMemory, *SSpikeMemory, 4);
                    c.addi(*SSpikeOut, *SSpikeOut, 4);
                    c.bne(*SSpikeMemory, *SSpikeMemoryEnd, spikeLoop);
                }
            }
        });

    // Dump to coe file
    //AppUtils::dumpCOE("mul.coe", code);

    
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

        const volatile uint32_t *wordData = reinterpret_cast<const volatile uint32_t*>(device.getDataMemory());
        checkOutput(wordData, outputSpikeArrayEnd, spikeArrayPtr, outputSpikeArrayPtr);
    }
    else {
        // Create simulated shared bus to connect the cores
        SharedBusSim sharedBus(1);
    
        // Create RISC-V core with instruction and scalar data
        RISCV riscV;
        riscV.setInstructions(code);
        riscV.getScalarDataMemory().setData(scalarInitData);
        
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);
        
        // Create simulated router
        RouterSim router(sharedBus, riscV.getSpikeDataMemory(), 0);
        riscV.setRouter(&router);

        // Run!
        if(!riscV.run()) {
            return 1;
        }

        const auto *wordData = reinterpret_cast<uint32_t*>(riscV.getScalarDataMemory().getData());
        checkOutput(wordData, outputSpikeArrayEnd, spikeArrayPtr, outputSpikeArrayPtr);
    
    }
    return 0;

}
