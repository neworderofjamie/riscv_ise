// Standard C++ includes
#include <fstream>
#include <numeric>
#include <random>
#include <tuple>
#include <thread>

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
constexpr uint32_t spikeArrayPtr = 32 * 4096;
constexpr size_t numCores = 2;

using CoreData = std::vector<std::tuple<uint32_t, std::vector<uint32_t>, std::thread>>;

void simThread(const std::vector<uint32_t> &code, const std::vector<uint8_t> &scalarInitData,
               SharedBusSim &sharedBus, uint32_t coreID, uint32_t spikeBitfield,
               uint32_t bitfieldPtr, uint32_t eventIDBasePtr, uint32_t outputSpikeArrayEnd, 
               uint32_t outputSpikeArrayPtr, std::vector<uint32_t> &receivedEvents)
{
    // Create RISC-V core with instruction and scalar data
    RISCV riscV;
    riscV.setInstructions(code);
    riscV.getScalarDataMemory().setData(scalarInitData);
    
    // Write bitfield and event ID base to memory
    auto *wordData = reinterpret_cast<uint32_t*>(riscV.getScalarDataMemory().getData());
    wordData[bitfieldPtr / 4] = spikeBitfield;
    wordData[eventIDBasePtr / 4] = (coreID << 14);

    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);
        
    // Create simulated DMA controller
    RouterSim router(sharedBus, riscV.getSpikeDataMemory(), coreID);
    riscV.setRouter(&router);
     
    // Run!
    if(!riscV.run()) {
        assert(false);
    }

    // Copy spikes received into vector
    const uint32_t num = (wordData[outputSpikeArrayEnd / 4] - spikeArrayPtr) / 4;
    std::copy_n(&wordData[outputSpikeArrayPtr / 4], num, std::back_inserter(receivedEvents));
}

void deviceThread(const std::vector<uint32_t> &code, const std::vector<uint8_t> &scalarInitData,
                  uint32_t coreID, uint32_t spikeBitfield,
                  uint32_t bitfieldPtr, uint32_t eventIDBasePtr, uint32_t outputSpikeArrayEnd, 
                  uint32_t outputSpikeArrayPtr, uint32_t readyFlagPtr, std::vector<uint32_t> &receivedEvents)
{
    LOGI << "Creating device (" << coreID << " / " << numCores << ")";
    Device device(coreID, numCores);
    LOGI << "Resetting";
    // Put core into reset state
    device.setEnabled(false);
    
    LOGI << "Copying instructions (" << code.size() * sizeof(uint32_t) << " bytes)";
    device.uploadCode(code);
    
    LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
    device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());
            
    // Hack with bitfield and eventID baseptr
    volatile uint32_t *wordData = reinterpret_cast<volatile uint32_t*>(device.getDataMemory());
    wordData[bitfieldPtr / 4] = spikeBitfield;
    wordData[eventIDBasePtr / 4] = (coreID << 14);

    LOGI << "Enabling";
    // Put core into running state
    device.setEnabled(true);
    LOGI << "Running";
    
    // Wait until ready flag
    device.waitOnNonZero(readyFlagPtr);
    device.setEnabled(false);
    LOGI << "Done";

    // Copy spikes received into vector
    const uint32_t num = (wordData[outputSpikeArrayEnd / 4] - spikeArrayPtr) / 4;
    for(uint32_t i = 0; i < num; i++) {
        receivedEvents.push_back((uint32_t)wordData[i + (outputSpikeArrayPtr / 4)]);
    }
}

void checkOutput(const CoreData &coreData)
{
    // Check all cores have received the same data
    const auto &core0Data = std::get<1>(coreData[0]);
    for(uint32_t i = 1; i < numCores; i++) {
        const auto &coreIData = std::get<1>(coreData[i]);
        assert(std::equal(core0Data.cbegin(), core0Data.cend(),
                          coreIData.cbegin(), coreIData.cend()));
    }

    // Make a copy of each core's bitfield
    std::vector<uint32_t> spikeBitfieldCopy;
    std::transform(coreData.cbegin(), coreData.cend(), std::back_inserter(spikeBitfieldCopy),
                   [](const auto &c){ return std::get<0>(c); });
    
    std::cout << std::get<1>(coreData[0]).size() << " spikes received" << std::endl;
    
    
    // Loop through events received by one core
    for(uint32_t s : std::get<1>(coreData[0])) {
        // Split event into core and neuron
        const uint32_t core = s >> 14;
        const uint32_t neuron = s & ((1 << 14) - 1);
        const uint32_t neuronBit = 1 << neuron;

        std::cout << std::hex << "\t" << s << std::endl;

        // Check bit is still set in core's bitmask
        assert(spikeBitfieldCopy[core] & neuronBit);

        // Clear BIT
        spikeBitfieldCopy[core] &= ~neuronBit;
    }

    // Check all bits have been cleared i.e. received spikes match bitfield
    for(uint32_t s : spikeBitfieldCopy) {
        assert(s == 0);
    }
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
    const uint32_t bitfieldPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t eventIDBasePtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t outputSpikeArrayEnd = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t outputSpikeArrayPtr = AppUtils::allocateScalarAndZero(64 * 4, scalarInitData);

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
            
            // Write MASTER_EVENT_ID_BASE
            {
                ALLOCATE_SCALAR(STmp);
                c.lw(*STmp, Reg::X0, eventIDBasePtr);

                c.csrw(CSR::MASTER_EVENT_ID_BASE, *STmp);
            }
            
            // Wait to give other cores time to start
            for(int i = 0; i < 100; i++) {
                c.nop();
            }
            
            // Write MASTER_EVENT_BITFIELD
            {
                ALLOCATE_SCALAR(STmp);
                c.lw(*STmp, Reg::X0, bitfieldPtr);   
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
        // Allocate vector with data for all cores
        CoreData coreData(numCores);
        
        // Loop through cores
        std::random_device d;
        for(uint32_t i = 0; i < numCores; i++) {
            // Generate spike bitfield for this core
            std::get<0>(coreData[i]) = d();

            // Create thread
            std::get<2>(coreData[i]) = std::thread(
                deviceThread, std::cref(code), std::cref(scalarInitData),
                i, std::get<0>(coreData[i]), bitfieldPtr, eventIDBasePtr, outputSpikeArrayEnd, 
                outputSpikeArrayPtr, readyFlagPtr, std::ref(std::get<1>(coreData[i])));
            
            // Name thread
            setThreadName(std::get<2>(coreData[i]), "Core " + std::to_string(i));
        }

        // Join all threads
        for(auto &c : coreData) {
            std::get<2>(c).join();
        }
        
        checkOutput(coreData);
    }
    else {
        // Create simulated shared bus to connect the cores
        SharedBusSim sharedBus(numCores);
    
        // Allocate vector with data for all cores
        CoreData coreData(numCores);

        // Loop through cores
        std::random_device d;
        for(uint32_t i = 0; i < numCores; i++) {
            // Generate spike bitfield for this core
            std::get<0>(coreData[i]) = d();

            // Create thread
            std::get<2>(coreData[i]) = std::thread(
                simThread, std::cref(code), std::cref(scalarInitData),
                std::ref(sharedBus), i, std::get<0>(coreData[i]),
                bitfieldPtr, eventIDBasePtr, outputSpikeArrayEnd, 
                outputSpikeArrayPtr, std::ref(std::get<1>(coreData[i])));
            
            // Name thread
            setThreadName(std::get<2>(coreData[i]), "Core " + std::to_string(i));
        }

        // Join all threads
        for(auto &c : coreData) {
            std::get<2>(c).join();
        }
        
        checkOutput(coreData);
    
    }
    return 0;

}
