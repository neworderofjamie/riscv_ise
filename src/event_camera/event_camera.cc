// Standard C++ includes
#include <fstream>
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
#include "common/genx320_bias.h"
#include "common/utils.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/event_injector_sim.h"
#include "ise/riscv.h"
#include "ise/router_sim.h"
#include "ise/shared_bus_sim.h"
#include "ise/vector_processor.h"

namespace
{
constexpr size_t numCores = 2;

using CoreData = std::vector<std::tuple<std::vector<uint32_t>, std::thread>>;

inline uint32_t buildTimestep(unsigned int t)
{
    assert(t < (1u << 31u));
    return (t | (1u << 31u));
}

inline uint32_t buildFeNNEvent(unsigned int x, unsigned int y,  bool p)
{
    assert(x < (1 << 9));
    assert(y < (1 << 9));
    return  ((p ? 1 : 0) << 18) | (x << 9) | y;
}

void simThread(const std::vector<uint32_t> &code, const std::vector<uint8_t> &scalarInitData,
               SharedBusSim &sharedBus, uint32_t coreID, uint32_t eventMemoryPtr, uint32_t eventEndPtr, uint32_t bufferOverflowCountPtr,
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

    const auto *wordData = reinterpret_cast<uint32_t*>(riscV.getScalarDataMemory().getData());
    const uint32_t bufferOverflowCount = wordData[bufferOverflowCountPtr / 4];
    if(bufferOverflowCount > 0) {
        LOGW << bufferOverflowCount << " events overflowed buffer";
    }

    // Copy spikes received into vector
    const uint32_t eventEnd = wordData[eventEndPtr / 4];
    std::copy(&wordData[eventMemoryPtr / 4], &wordData[eventEnd / 4], std::back_inserter(receivedEvents));
}

void simEventInjectorThread(SharedBusSim &sharedBus, uint32_t coreID, const std::vector<uint32_t> &data)
{
    // Create event injector
    EventInjectorSim eventInjector(sharedBus, data, coreID);

    // Keep ticking event injector until it runs out of data
    while(eventInjector.tick());
}

void deviceThread(const std::vector<uint32_t> &code, const std::vector<uint8_t> &scalarInitData,
                  uint32_t coreID, uint32_t eventCounterPtr, uint32_t bufferOverflowCountPtr, uint32_t readyFlagPtr,
                  std::vector<uint32_t> &receivedEvents, Barrier &barrier)
{
    LOGI << "Creating device (" << coreID << " / " << numCores << ")";
    Device device(coreID, numCores);
    
    std::unique_ptr<DeviceControl> deviceControl;
    if(coreID == 0) {
        deviceControl = std::make_unique<DeviceControl>(numCores);

        // Set GenX320 bias to 'low'
        deviceControl->getGenX320()->setBias<Bias::Diff>(41);
        deviceControl->getGenX320()->setBias<Bias::DiffOff>(19);
        deviceControl->getGenX320()->setBias<Bias::DiffOn>(24);
        deviceControl->getGenX320()->setBias<Bias::FO>(19);
        deviceControl->getGenX320()->setBias<Bias::HPF>(0);
        deviceControl->getGenX320()->setBias<Bias::Refr>(0);

        // Most we can handle is 500KEvents/sec
        deviceControl->getGenX320()->enableERC(500000);
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

    volatile const uint32_t *wordData = reinterpret_cast<volatile uint32_t*>(device.getDataMemory());
    const uint32_t bufferOverflowCount = wordData[bufferOverflowCountPtr / 4];
    if(bufferOverflowCount > 0) {
        LOGW << bufferOverflowCount << " events overflowed buffer";
    }

    const uint32_t eventCount = wordData[eventCounterPtr / 4];
    LOGI << eventCount << " events received";
}

void checkOutput(const std::vector<uint32_t> &coreOutput, const std::vector<uint32_t> &inputEvents)
{
    constexpr uint32_t timestampBit = (1u << 31u);
    auto coreOutputIter = coreOutput.cbegin();
    auto correctIter = inputEvents.cbegin();

    std::vector<uint32_t> correctTimestepEvents;
    std::vector<uint32_t> outputTimestepEvents;
    std::vector<uint32_t> missingCorrectEvents;
    std::vector<uint32_t> missingOutputEvents;
    while(coreOutputIter != coreOutput.cend() && correctIter != inputEvents.cend()) {
        // Assert that both iterators point to timestamps
        assert(*coreOutputIter & timestampBit);
        assert(*correctIter & timestampBit);

        // Assert they are the same
        assert(*coreOutputIter == *correctIter);
        const uint32_t timestep = (*correctIter & ~(1u << 31u));

        // Advance
        coreOutputIter++;
        correctIter++;

        // Find end of timestep in correct data, copy into vector and sort
        correctTimestepEvents.clear();
        const auto correctEndOfTimestep = std::find_if(correctIter, inputEvents.cend(),
                                                       [](uint32_t e){ return (e & timestampBit); });
        std::copy(correctIter, correctEndOfTimestep, std::back_inserter(correctTimestepEvents));
        std::sort(correctTimestepEvents.begin(), correctTimestepEvents.end());
        correctIter = correctEndOfTimestep;

        // Find end of timestep in output data, copy into vector and sort
        outputTimestepEvents.clear();
        const auto outputEndOfTimestep = std::find_if(coreOutputIter, coreOutput.cend(),
                                                      [](uint32_t e){ return (e & timestampBit); });
        std::copy(coreOutputIter, outputEndOfTimestep, std::back_inserter(outputTimestepEvents));
        std::sort(outputTimestepEvents.begin(), outputTimestepEvents.end());
        coreOutputIter = outputEndOfTimestep;
        
        missingCorrectEvents.clear();
        std::set_difference(correctTimestepEvents.cbegin(), correctTimestepEvents.cend(),
                            outputTimestepEvents.cbegin(), outputTimestepEvents.cend(),
                            std::back_inserter(missingCorrectEvents));

        missingOutputEvents.clear();
        std::set_difference(outputTimestepEvents.cbegin(), outputTimestepEvents.cend(),
                            correctTimestepEvents.cbegin(), correctTimestepEvents.cend(),
                            std::back_inserter(missingOutputEvents));
        
        if (!missingCorrectEvents.empty()) {
            PLOGW << missingCorrectEvents.size() << "/" << correctTimestepEvents.size() << " events missing from core output at time " << timestep;
        }

        if (!missingOutputEvents.empty()) {
            PLOGE << missingOutputEvents.size() << "/" << outputTimestepEvents.size() << " events in output which weren't in input at time " << timestep;
        }

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
    unsigned int downsampleShift = 0;

    CLI::App app{"Blank example"};
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");
    app.add_flag("-s,--downsample-shift", downsampleShift, "How much should resolution be shifted down?");
    app.add_option("-c,--clock-speed", clockSpeedMhz, "What clock speed is the device running at [Mhz]?");

    CLI11_PARSE(app, argc, argv);
    
    // Determine resolution
    const unsigned int downsampleResolution = 320 >> downsampleShift;
    const uint32_t downsampleMask = (1 << (9 - downsampleShift)) - 1;

    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t eventCounterPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t bufferOverflowCountPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, 
            ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeEnd);
            ALLOCATE_SCALAR(SNumEvents);
        
            // Enable performance counters
            // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
            c.csrw(CSR::MCOUNTINHIBIT, Reg::X0);

            // Start time at 0
            c.li(*STime, 0);
            c.li(*STimeEnd, 10000);
            c.li(*SNumEvents, 0);

            auto timeLoop = c.L();
            {
                ALLOCATE_SCALAR(SLoopStartCycleLow);
                ALLOCATE_SCALAR(SLoopStartCycleHigh);

                // Wait for all events to be communicated
                // **NOTE** this is only necessary to swap buffers and, in simulation, to synchronise event injector
                AssemblerUtils::generateRouterBarrier(c, scalarRegisterAllocator, 
                                                      device ? numCores : (numCores + 1));

                // Read cycle count at start of loop
                c.csrr(*SLoopStartCycleLow, CSR::MCYCLE);
                c.csrr(*SLoopStartCycleHigh, CSR::MCYCLEH);

                {
                    Label eventLoopEnd;

                    // Get start and end of event buffer
                    ALLOCATE_SCALAR(SRouterEventBuffer);
                    ALLOCATE_SCALAR(SRouterEventBufferEnd);
                    c.csrr(*SRouterEventBuffer, CSR::SLAVE_EVENT_START_ADDRESS);
                    c.csrr(*SRouterEventBufferEnd, CSR::SLAVE_EVENT_END_ADDRESS);
                    
                    c.sub(*SRouterEventBuffer, *SRouterEventBufferEnd, *SRouterEventBuffer);
                    c.add(*SNumEvents, *SNumEvents, *SRouterEventBuffer);
                    
                }
                

                // Wait until 1ms of clock cycles has elapsed since start of loop
                if(device) {
                    AssemblerUtils::generateWaitElapsedCycles(c, scalarRegisterAllocator, 
                                                              *SLoopStartCycleLow, *SLoopStartCycleHigh, 
                                                              clockSpeedMhz * 1000);
                }

                // Increment time
                c.addi(*STime, *STime, 1);
                
                // If we're on device, loop
                c.bne(*STime, *STimeEnd, timeLoop);
            }

            // Store buffer overflow count
            {
                ALLOCATE_SCALAR(STmp);
                c.csrr(*STmp, CSR::SLAVE_BUFFER_OVERFLOW_COUNT);
                c.sw(*STmp, Reg::X0, bufferOverflowCountPtr);
            }
            // Store event cout
            c.sw(*SNumEvents, Reg::X0, eventCounterPtr);
        });

    // Dump to coe file
    AppUtils::dumpCOE("event_camera.coe", code);
    
    // Load input sequence
    const auto spikeInjectData = AppUtils::loadBinaryData<uint32_t>("courtyard.bin");

    if(device) {
        // Allocate vector with data for all cores
        CoreData coreData(numCores);
        
        Barrier barrier(numCores);
        // Loop through cores
        for(uint32_t i = 0; i < numCores; i++) {
            // Create thread
            std::get<1>(coreData[i]) = std::thread(
                deviceThread, std::cref(code), std::cref(scalarInitData),
                i, eventCounterPtr, bufferOverflowCountPtr, readyFlagPtr, 
                std::ref(std::get<0>(coreData[i])), std::ref(barrier));
            
            // Name thread
            setThreadName(std::get<1>(coreData[i]), "Core " + std::to_string(i));
        }

        // Join all threads
        for(auto &c : coreData) {
            std::get<1>(c).join();
        }
        
        {
            std::ofstream output("recorded_spikes.bin", std::ios::binary);
            output.write(reinterpret_cast<const char*>(std::get<0>(coreData[0]).data()), std::get<0>(coreData[0]).size() * 4);
        }
    }
    /*else {
        // Create simulated shared bus to connect the cores
        SharedBusSim sharedBus(numCores + 1);
    
        // Allocate vector with data for all cores
        CoreData coreData(numCores);

        // Loop through cores
        for(uint32_t i = 0; i < numCores; i++) {
            // Create thread
            std::get<1>(coreData[i]) = std::thread(
                simThread, std::cref(code), std::cref(scalarInitData),
                std::ref(sharedBus), i, eventMemoryPtr, eventEndPtr, bufferOverflowCountPtr,
                std::ref(std::get<0>(coreData[i])));
            
            // Name thread
            setThreadName(std::get<1>(coreData[i]), "Core " + std::to_string(i));
        }

        // Create another thread to inject spikes
        std::thread injectorThread(simEventInjectorThread, std::ref(sharedBus), numCores, spikeInjectData);
        setThreadName(injectorThread, "Spike injector");

        // Join all threads
        for(auto &c : coreData) {
            std::get<1>(c).join();
        }
        injectorThread.join();

        checkOutput(std::get<0>(coreData[0]), spikeInjectData);
        {
            std::ofstream output("recorded_spikes.bin", std::ios::binary);
            output.write(reinterpret_cast<const char*>(std::get<0>(coreData[0]).data()), std::get<0>(coreData[0]).size() * 4);
        }
    }*/
    return 0;

}
