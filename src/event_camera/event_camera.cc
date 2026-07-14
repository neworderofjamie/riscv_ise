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
#include "common/barrier.h"
#include "common/device.h"
#include "common/device_control.h"
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
constexpr size_t numCores = 1;

using CoreData = std::vector<std::tuple<std::vector<uint32_t>, std::thread>>;

void simThread(const std::vector<uint32_t> &code, const std::vector<uint8_t> &scalarInitData,
               SharedBusSim &sharedBus, uint32_t coreID, uint32_t eventMemoryPtr, 
               std::vector<uint32_t> &receivedEvents)
{
    // Create RISC-V core with instruction and scalar data
    RISCV riscV;
    riscV.setInstructions(code);
    riscV.getScalarDataMemory().setData(scalarInitData);

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
    const auto *wordData = reinterpret_cast<uint32_t*>(riscV.getScalarDataMemory().getData());
    std::copy_n(&wordData[eventMemoryPtr / 4], ((4096 * 32) - eventMemoryPtr) / 4, std::back_inserter(receivedEvents));
}

void deviceThread(const std::vector<uint32_t> &code, const std::vector<uint8_t> &scalarInitData,
                  uint32_t coreID, uint32_t eventMemoryPtr, uint32_t readyFlagPtr, 
                  std::vector<uint32_t> &receivedEvents, Barrier &barrier)
{
    LOGI << "Creating device (" << coreID << " / " << numCores << ")";
    Device device(coreID, numCores);
    
    std::unique_ptr<DeviceControl> deviceControl;
    if(coreID == 0) {
        deviceControl = std::make_unique<DeviceControl>(numCores);
    }

    LOGI << "Resetting";
    // Put core into reset state
    barrier.wait();
    if(coreID == 0) {
        deviceControl->setEnabled(false);
    }
    
    LOGI << "Copying instructions (" << code.size() * sizeof(uint32_t) << " bytes)";
    device.uploadCode(code);
    
    LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
    device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());

    barrier.wait();
    LOGI << "Enabling";

    // Put core into running state and start streaming data from camera and 
    if(coreID == 0) {
        deviceControl->setEnabled(true);
        deviceControl->getGenX320()->startStreaming();
    }
    LOGI << "Running " << readyFlagPtr;
    
    // Wait until ready flag
    device.waitOnNonZero(readyFlagPtr);
    LOGI << "Done";
    barrier.wait();

    // Stop streaming data from camera and put core in disabled state
    if(coreID == 0) {
        deviceControl->getGenX320()->stopStreaming();
        deviceControl->setEnabled(false);
    }
    LOGI << "Cores disabled";

    // Copy spikes received into vector
    volatile const uint32_t *wordData = reinterpret_cast<volatile uint32_t*>(device.getDataMemory());

    for(uint32_t i = eventMemoryPtr; i < (4096 * 32); i++) {
        receivedEvents.push_back((uint32_t)wordData[i / 4]);
    }
}
}

int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    bool device = false;
    int clockSpeedMhz = 166;

    CLI::App app{"Blank example"};
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");
    app.add_option("-c,--clock-speed", clockSpeedMhz, "What clock speed is the device running at [Mhz]?");

    CLI11_PARSE(app, argc, argv);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t eventMemoryPtr = AppUtils::allocateScalarAndZero((4096 * 32) - scalarInitData.size(), scalarInitData);
    
    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator&, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeMarker);
            ALLOCATE_SCALAR(SEventMemory);
            ALLOCATE_SCALAR(SEventMemoryEnd);

            // Start time at 0
            c.li(*STime, 0);

            // Bit to mark timestamps with
            c.li(*STimeMarker, 1 << 31);

            // Load spike memory pointers
            c.li(*SEventMemory, eventMemoryPtr);
            c.li(*SEventMemoryEnd, 4096 * 32);

            Label timeLoopEnd;
            auto timeLoop = c.L();
            {
                ALLOCATE_SCALAR(SLoopStartCycleLow);
                ALLOCATE_SCALAR(SLoopStartCycleHigh);

                

                // Read cycle count at start of loop
                c.csrr(*SLoopStartCycleLow, CSR::MCYCLE);
                c.csrr(*SLoopStartCycleHigh, CSR::MCYCLEH);

                // Swap router buffers
                c.csrwi(CSR::SLAVE_SWAP_BUFFER, 1);

                {
                    Label eventLoopEnd;

                    // Get start and end of event buffer
                    ALLOCATE_SCALAR(SEventBuffer);
                    ALLOCATE_SCALAR(SEventBufferEnd);
                    c.csrr(*SEventBuffer, CSR::SLAVE_EVENT_START_ADDRESS);
                    c.csrr(*SEventBufferEnd, CSR::SLAVE_EVENT_END_ADDRESS);
                    
                    // If there aren't any events goto end
                    c.beq(*SEventBuffer, *SEventBufferEnd, eventLoopEnd);

                    // Write time with top bit set to spike memory
                    {
                        ALLOCATE_SCALAR(STmp);
                        c.or_(*STmp, *STime, *STimeMarker);
                        c.sw(*STmp, *SEventMemory);

                        // Advance spike memory pointer and goto end if end of memory reached
                        c.addi(*SEventMemory, *SEventMemory, 4);
                        c.beq(*SEventMemory, *SEventMemoryEnd, timeLoopEnd);
                    }

                    // While (spikeBuffer != spikeBufferEnd
                    auto eventLoop = c.L();
                    
                    c.beq(*SEventBuffer, *SEventBufferEnd, eventLoopEnd);
                    {
                        // Load spike from buffer and incrememnt pointer
                        ALLOCATE_SCALAR(SEvent);
                        c.lw(*SEvent, *SEventBuffer);
                        c.addi(*SEventBuffer, *SEventBuffer, 4);

                        // Write event to spike memory
                        c.sw(*SEvent, *SEventMemory);

                        // Advance spike memory pointer and goto end if end of memory reached
                        c.addi(*SEventMemory, *SEventMemory, 4);
                        c.beq(*SEventMemory, *SEventMemoryEnd, timeLoopEnd);

                        // Next event
                        c.j_(eventLoop);
                    }
                    c.L(eventLoopEnd);
                }

                // Increment time
                c.addi(*STime, *STime, 1);

                // Wait until 1ms of clock cycles has elapsed since start of loop
                AssemblerUtils::generateWaitElapsedCycles(c, scalarRegisterAllocator, 
                                                          *SLoopStartCycleLow, *SLoopStartCycleHigh, 
                                                          clockSpeedMhz * 1000);

                // Loop
                c.j_(timeLoop);
            }
            c.L(timeLoopEnd);
        });

    // Dump to coe file
    //AppUtils::dumpCOE("mul.coe", code);

    
    if(device) {
        // Allocate vector with data for all cores
        CoreData coreData(numCores);
        
        Barrier barrier(numCores);
        // Loop through cores
        for(uint32_t i = 0; i < numCores; i++) {
            // Create thread
            std::get<1>(coreData[i]) = std::thread(
                deviceThread, std::cref(code), std::cref(scalarInitData),
                i, eventMemoryPtr, readyFlagPtr, 
                std::ref(std::get<0>(coreData[i])), std::ref(barrier));
            
            // Name thread
            setThreadName(std::get<1>(coreData[i]), "Core " + std::to_string(i));
        }

        // Join all threads
        for(auto &c : coreData) {
            std::get<1>(c).join();
        }
    }
    else {
        // Create simulated shared bus to connect the cores
        SharedBusSim sharedBus(numCores);
    
        // Allocate vector with data for all cores
        CoreData coreData(numCores);

        // Loop through cores
        for(uint32_t i = 0; i < numCores; i++) {
            // Create thread
            std::get<1>(coreData[i]) = std::thread(
                simThread, std::cref(code), std::cref(scalarInitData),
                std::ref(sharedBus), i, eventMemoryPtr,
                std::ref(std::get<0>(coreData[i])));
            
            // Name thread
            setThreadName(std::get<1>(coreData[i]), "Core " + std::to_string(i));
        }

        // Join all threads
        for(auto &c : coreData) {
            std::get<1>(c).join();
        }
    }
    return 0;

}
