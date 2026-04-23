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
                 uint32_t evenSpikeBufferPtr, uint32_t oddSpikeBufferPtr)
{
    auto *evenSpike = wordData + (evenSpikeBufferPtr / 4);
    auto *oddSpike = wordData + (oddSpikeBufferPtr / 4);
    
    size_t good = 0;
    for(uint32_t s : spikes) {
        LOGI << s << ", " << *evenSpike << ", " << *oddSpike;
        // Even
        if((s % 2) == 0) {
            if(*evenSpike++ == s) {
                good++;
            }
        }
        else {
            if(*oddSpike++ == s) {
                good++;
            }
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
    const uint32_t evenSpikeBufferPtr = AppUtils::allocateScalarAndZero(4 * 32, scalarInitData);
    const uint32_t oddSpikeBufferPtr = AppUtils::allocateScalarAndZero(4 * 32, scalarInitData);
    const uint32_t evenSpikeBufferEndPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t oddSpikeBufferEndPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    
    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_SCALAR(SSpike);
            ALLOCATE_SCALAR(SSpikeBuffer);
            ALLOCATE_SCALAR(SSpikeBufferEnd);
            ALLOCATE_SCALAR(SOddSpikeBuffer);
            ALLOCATE_SCALAR(SEvenSpikeBuffer);

            // Labels
            Label spikeLoop;
            Label jumpTable;
            Label evenSpike;
            Label oddSpike;
            Label nextSpike;
        
            c.li(*SSpikeBuffer, spikeBufferPtr);
            c.li(*SOddSpikeBuffer, oddSpikeBufferPtr);
            c.li(*SEvenSpikeBuffer, evenSpikeBufferPtr);
            c.addi(*SSpikeBufferEnd, *SSpikeBuffer, 32 * 4);
        
            // Jump over jump table etc to start of loop
            c.j_(spikeLoop);

            // Define jump table for 
            c.L(jumpTable);
            if(device) {
                c.nop();
            }
            c.j_(evenSpike);
            c.j_(oddSpike);
            
            // ----------------------------------   -----------------------------
            // Even spikes
            // ---------------------------------------------------------------
            {
                c.L(evenSpike);
                c.sw(*SSpike, *SEvenSpikeBuffer);
                c.addi(*SEvenSpikeBuffer, *SEvenSpikeBuffer, 4);
                c.j_(nextSpike);
            }

            // ---------------------------------------------------------------
            // Odd spikes
            // ---------------------------------------------------------------
            {
                c.L(oddSpike);
                c.sw(*SSpike, *SOddSpikeBuffer);
                c.addi(*SOddSpikeBuffer, *SOddSpikeBuffer, 4);
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
                    ALLOCATE_SCALAR(SPopulationID);
                    c.andi(*SPopulationID, *SSpike, 1);
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
            c.sw(*SOddSpikeBuffer, Reg::X0, oddSpikeBufferEndPtr);
            c.sw(*SEvenSpikeBuffer, Reg::X0, evenSpikeBufferEndPtr);
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
        checkOutput(spikes, wordData, evenSpikeBufferPtr, oddSpikeBufferPtr);

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
        checkOutput(spikes, wordData, evenSpikeBufferPtr, oddSpikeBufferPtr);
    }
    return 0;

}
