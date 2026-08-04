// Standard C++ includes
#include <fstream>
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
#include "common/device_control.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"

namespace
{
void checkOutput(const std::vector<uint32_t> &spikes, volatile const uint32_t *wordData,
                 uint32_t rem0SpikeBufferPtr, uint32_t rem1SpikeBufferPtr, 
                 uint32_t rem2SpikeBufferPtr, uint32_t rem3SpikeBufferPtr)
{
    volatile const uint32_t *remSpikes[4] = {
        wordData + (rem0SpikeBufferPtr / 4),
        wordData + (rem1SpikeBufferPtr / 4),
        wordData + (rem2SpikeBufferPtr / 4),
        wordData + (rem3SpikeBufferPtr / 4)};
    
    size_t good = 0;
    for(uint32_t s : spikes) {
        LOGI << s << ", " << *remSpikes[0] << ", " << *remSpikes[1] << ", " << *remSpikes[2] << ", " << *remSpikes[3];

        if(*remSpikes[s % 4]++ == s) {
            good++;
        }
    }
    
    LOGI << good << " / " << spikes.size();
    assert(good == spikes.size());
}
}

int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    bool device = false;
    int numCores = 1;
    int core = 0;

    CLI::App app{"Blank example"};
    app.add_option("-n,--num-cores", numCores, "Number of cores FeNN system has");
    app.add_option("-c,--core", core, "Which core to run on");
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");

    CLI11_PARSE(app, argc, argv);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t spikeBufferPtr = AppUtils::allocateScalarAndZero(4 * 32, scalarInitData);
    const uint32_t rem0SpikeBufferPtr = AppUtils::allocateScalarAndZero(4 * 32, scalarInitData);
    const uint32_t rem1SpikeBufferPtr = AppUtils::allocateScalarAndZero(4 * 32, scalarInitData);
    const uint32_t rem2SpikeBufferPtr = AppUtils::allocateScalarAndZero(4 * 32, scalarInitData);
    const uint32_t rem3SpikeBufferPtr = AppUtils::allocateScalarAndZero(4 * 32, scalarInitData);
    const uint32_t rem0SpikeBufferEndPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t rem1SpikeBufferEndPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t rem2SpikeBufferEndPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t rem3SpikeBufferEndPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    
    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_SCALAR(SSpike);
            ALLOCATE_SCALAR(SSpikeBuffer);
            ALLOCATE_SCALAR(SSpikeBufferEnd);
            ALLOCATE_SCALAR(SRem0SpikeBuffer);
            ALLOCATE_SCALAR(SRem1SpikeBuffer);
            ALLOCATE_SCALAR(SRem2SpikeBuffer);
            ALLOCATE_SCALAR(SRem3SpikeBuffer);

            // Labels
            Label spikeLoop;
            Label jumpTable;
            Label rem0Spike;
            Label rem1Spike;
            Label rem2Spike;
            Label rem3Spike;
            Label nextSpike;
        
            c.li(*SSpikeBuffer, spikeBufferPtr);
            c.li(*SRem0SpikeBuffer, rem0SpikeBufferPtr);
            c.li(*SRem1SpikeBuffer, rem1SpikeBufferPtr);
            c.li(*SRem2SpikeBuffer, rem2SpikeBufferPtr);
            c.li(*SRem3SpikeBuffer, rem3SpikeBufferPtr);
            c.addi(*SSpikeBufferEnd, *SSpikeBuffer, 32 * 4);
        
            // Jump over jump table etc to start of loop
            c.j_(spikeLoop);

            // Define jump table for 
            c.L(jumpTable);
            if(device) {
                c.nop();
            }
            c.j_(rem0Spike);
            c.j_(rem1Spike);
            c.j_(rem2Spike);
            c.j_(rem3Spike);
            
            // ----------------------------------   -----------------------------
            // Rem0 spikes
            // ---------------------------------------------------------------
            {
                c.L(rem0Spike);
                c.sw(*SSpike, *SRem0SpikeBuffer);
                c.addi(*SRem0SpikeBuffer, *SRem0SpikeBuffer, 4);
                c.j_(nextSpike);
            }

            // ----------------------------------   -----------------------------
            // Rem1 spikes
            // ---------------------------------------------------------------
            {
                c.L(rem1Spike);
                c.sw(*SSpike, *SRem1SpikeBuffer);
                c.addi(*SRem1SpikeBuffer, *SRem1SpikeBuffer, 4);
                c.j_(nextSpike);
            }

            // ----------------------------------   -----------------------------
            // Rem2 spikes
            // ---------------------------------------------------------------
            {
                c.L(rem2Spike);
                c.sw(*SSpike, *SRem2SpikeBuffer);
                c.addi(*SRem2SpikeBuffer, *SRem2SpikeBuffer, 4);
                c.j_(nextSpike);
            }

            // ----------------------------------   -----------------------------
            // Rem3 spikes
            // ---------------------------------------------------------------
            {
                c.L(rem3Spike);
                c.sw(*SSpike, *SRem3SpikeBuffer);
                c.addi(*SRem3SpikeBuffer, *SRem3SpikeBuffer, 4);
                c.j_(nextSpike);
            }

            Label spikeLoopEnd;

            // While (spikeBuffer != spikeBufferEnd
            c.L(spikeLoop);
            c.beq(*SSpikeBuffer, *SSpikeBufferEnd, spikeLoopEnd);
            {
                // Load spike from buffer
                c.lw(*SSpike, *SSpikeBuffer);

                {
                    // Extract population ID
                    // **NOTE** multiple by 4 to get jump bytes
                    ALLOCATE_SCALAR(SPopulationID);
                    c.andi(*SPopulationID, *SSpike, 0b11);
                    c.slli(*SPopulationID, *SPopulationID, 2);

                    // Jump to correct population handler
                    c.jalr(Reg::X0, *SPopulationID, jumpTable.getAddress());
                }
                c.L(nextSpike);

                // Loop until spikes are processed
                c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);
                c.j_(spikeLoop);
            }
            c.L(spikeLoopEnd);

            // Store odd and even end pointers
            c.sw(*SRem0SpikeBuffer, Reg::X0, rem0SpikeBufferEndPtr);
            c.sw(*SRem1SpikeBuffer, Reg::X0, rem1SpikeBufferEndPtr);
            c.sw(*SRem2SpikeBuffer, Reg::X0, rem2SpikeBufferEndPtr);
            c.sw(*SRem3SpikeBuffer, Reg::X0, rem3SpikeBufferEndPtr);
        });

    // Dump to coe file
    AppUtils::dumpCOE("blank_jump.coe", code);

    // Generate vector of random spikes
    std::vector<uint32_t> spikes(32);
    std::random_device d;
    std::generate(spikes.begin(), spikes.end(),
                  [&d](){ return d(); });
    
    // Copy spikes into memory
    std::memcpy(scalarInitData.data() + spikeBufferPtr, spikes.data(), 32 * 4);

    // Dump initial data to coe file
    std::vector<uint32_t> wordData(scalarInitData.size() / 4);
    std::memcpy(wordData.data(), scalarInitData.data(), scalarInitData.size());
    AppUtils::dumpCOE("blank_jump_data.coe", wordData);

    if(device) {
        LOGI << "Creating device";
        DeviceControl deviceControl(numCores);
        Device device(core, numCores);

        LOGI << "Resetting";
        // Put core into reset state
        deviceControl.setEnabled(false);
        
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
        LOGI << "Done";

        volatile uint32_t *wordData = reinterpret_cast<volatile uint32_t*>(device.getDataMemory());
        checkOutput(spikes, wordData, rem0SpikeBufferPtr, rem1SpikeBufferPtr, 
                    rem2SpikeBufferPtr, rem3SpikeBufferPtr);

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

        auto *wordData = reinterpret_cast<uint32_t*>(riscV.getScalarDataMemory().getData());
        checkOutput(spikes, wordData, rem0SpikeBufferPtr, rem1SpikeBufferPtr, 
                    rem2SpikeBufferPtr, rem3SpikeBufferPtr);
    }
    return 0;

}
