// Standard C++ includes
#include <fstream>
#include <random>
#include <tuple>
#include <thread>

// Standard C includes
#include <cassert>

// PLOG includes
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>

// Common includes
#include "third_party/CLI11.hpp"
#include "common/barrier.h"
#include "common/utils.h"

// FeNN common includes
#include "fenn/common/app_utils.h"
#include "fenn/common/device.h"
#include "fenn/common/device_control.h"
#include "fenn/common/genx320_bias.h"
#include "fenn/common/logging.h"

// FeNN assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/assembler_utils.h"
#include "fenn/assembler/register_allocator.h"

// FeNN ISE includes
#include "fenn/ise/event_injector_sim.h"
#include "fenn/ise/riscv.h"
#include "fenn/ise/router_sim.h"
#include "fenn/ise/shared_bus_sim.h"
#include "fenn/ise/vector_processor.h"

using namespace FeNN;

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
               ISE::SharedBusSim &sharedBus, uint32_t coreID, uint32_t eventMemoryPtr, uint32_t eventEndPtr, uint32_t bufferOverflowCountPtr,
               std::vector<uint32_t> &receivedEvents)
{
    // Create RISC-V core with instruction and scalar data
    ISE::RISCV riscV(coreID);
    riscV.setInstructions(code);
    riscV.getScalarDataMemory().setData(scalarInitData);

    // Add vector co-processor
    riscV.addCoprocessor<ISE::VectorProcessor>(FeNN::Common::vectorQuadrant);
        
    // Create simulated DMA controller
    ISE::RouterSim router(sharedBus, riscV.getSpikeDataMemory(), coreID);
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

void simEventInjectorThread(ISE::SharedBusSim &sharedBus, uint32_t coreID, const std::vector<uint32_t> &data)
{
    // Create event injector
    ISE::EventInjectorSim eventInjector(sharedBus, data, coreID);

    // Keep ticking event injector until it runs out of data
    while(eventInjector.tick());
}

void deviceThread(const std::vector<uint32_t> &code, const std::vector<uint8_t> &scalarInitData,
                  uint32_t coreID, uint32_t eventCounterPtr, uint32_t minIDPtr, 
                  uint32_t maxIDPtr, uint32_t bufferOverflowCountPtr, uint32_t readyFlagPtr,
                  std::vector<uint32_t> &receivedEvents, ::Common::Barrier &barrier)
{
    LOGI << "Creating device (" << coreID << " / " << numCores << ")";
    FeNN::Common::Device device(coreID, numCores);
    
    std::unique_ptr<FeNN::Common::DeviceControl> deviceControl;
    if(coreID == 0) {
        deviceControl = std::make_unique<FeNN::Common::DeviceControl>(numCores);

        // Set GenX320 bias to 'low'
        deviceControl->getGenX320()->setBias<FeNN::Common::Bias::Diff>(41);
        deviceControl->getGenX320()->setBias<FeNN::Common::Bias::DiffOff>(19);
        deviceControl->getGenX320()->setBias<FeNN::Common::Bias::DiffOn>(24);
        deviceControl->getGenX320()->setBias<FeNN::Common::Bias::FO>(19);
        deviceControl->getGenX320()->setBias<FeNN::Common::Bias::HPF>(0);
        deviceControl->getGenX320()->setBias<FeNN::Common::Bias::Refr>(0);

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
    LOGI << wordData[eventCounterPtr / 4] << " events received";
    LOGI << wordData[minIDPtr / 4] << " min event ID received";
    LOGI << wordData[maxIDPtr / 4] << " max event ID received";
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
    bool device = false;
    int clockSpeedMhz = 166;
    unsigned int downsampleShift = 0;
    plog::Severity logSeverity = plog::debug;
    
    CLI::App app{"Event camera example"};
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");
    app.add_option_function<std::string>("-l,--log-level", 
                                         [&logSeverity](const std::string &l)
                                         {
                                             logSeverity = plog::severityFromString(l.c_str());
                                         });
    app.add_flag("-s,--downsample-shift", downsampleShift, "How much should resolution be shifted down?");
    app.add_option("-c,--clock-speed", clockSpeedMhz, "What clock speed is the device running at [Mhz]?");

    CLI11_PARSE(app, argc, argv);
    
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    ::Common::Logging::init(logSeverity, logSeverity, 
                            &consoleAppender, &consoleAppender);
    FeNN::Common::Logging::init(logSeverity, logSeverity, logSeverity, logSeverity, logSeverity,
                                &consoleAppender, &consoleAppender, &consoleAppender, &consoleAppender, &consoleAppender);


    // Determine resolution
    const unsigned int downsampleResolution = 320 >> downsampleShift;
    const uint32_t downsampleMask = (1 << (9 - downsampleShift)) - 1;

    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = FeNN::Common::AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t eventCounterPtr = FeNN::Common::AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t minIDPtr = FeNN::Common::AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t maxIDPtr = FeNN::Common::AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t bufferOverflowCountPtr = FeNN::Common::AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Generate code
    const auto code = Assembler::Utils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](Assembler::CodeGenerator &c, Assembler::VectorRegisterAllocator &vectorRegisterAllocator, 
            Assembler::ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeEnd);
            ALLOCATE_SCALAR(SNumEvents);
            ALLOCATE_SCALAR(SMinEventID);
            ALLOCATE_SCALAR(SMaxEventID);
        
            // Enable performance counters
            // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
            c.csrw(FeNN::Common::CSR::MCOUNTINHIBIT, FeNN::Common::Reg::X0);

            // Start time at 0
            c.li(*STime, 0);
            c.li(*STimeEnd, 10000);
            c.li(*SNumEvents, 0);
            c.li(*SMinEventID, 0);
            c.li(*SMaxEventID, 0xFFFFFFFF);

            auto timeLoop = c.L();
            {
                ALLOCATE_SCALAR(SLoopStartCycleLow);
                ALLOCATE_SCALAR(SLoopStartCycleHigh);

                // Wait for all events to be communicated
                // **NOTE** this is only necessary to swap buffers and, in simulation, to synchronise event injector
                Assembler::Utils::generateRouterBarrier(c, scalarRegisterAllocator, 
                                                        device ? numCores : (numCores + 1));

                // Read cycle count at start of loop
                c.csrr(*SLoopStartCycleLow, FeNN::Common::CSR::MCYCLE);
                c.csrr(*SLoopStartCycleHigh, FeNN::Common::CSR::MCYCLEH);

                {
                    

                    // Get start and end of event buffer
                    ALLOCATE_SCALAR(SRouterEventBuffer);
                    ALLOCATE_SCALAR(SRouterEventBufferEnd);
                    c.csrr(*SRouterEventBuffer, FeNN::Common::CSR::SLAVE_EVENT_START_ADDRESS);
                    c.csrr(*SRouterEventBufferEnd, FeNN::Common::CSR::SLAVE_EVENT_END_ADDRESS);
                    
                    // Calculate number of events and add to total
                    c.sub(*SRouterEventBuffer, *SRouterEventBufferEnd, *SRouterEventBuffer);
                    c.add(*SNumEvents, *SNumEvents, *SRouterEventBuffer);

                    auto eventLoopEnd = Assembler::createLabel();
                    auto eventLoopStart = c.L();
                    c.beq(*SRouterEventBuffer, *SRouterEventBufferEnd, eventLoopEnd);
                    {
                        ALLOCATE_SCALAR(SEvent);

                        // Load event
                        c.lw(*SEvent, *SRouterEventBuffer, 0);
                        c.addi(*SRouterEventBuffer, *SRouterEventBuffer, 4);

                        // Update min and max
                        c.minu(*SMinEventID, *SMinEventID, *SEvent);
                        c.maxu(*SMaxEventID, *SMaxEventID, *SEvent);

                        c.j_(eventLoopStart);
                    }
                    c.L(eventLoopEnd);
                }
                

                // Wait until 1ms of clock cycles has elapsed since start of loop
                if(device) {
                    Assembler::Utils::generateWaitElapsedCycles(c, scalarRegisterAllocator, 
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
                c.csrr(*STmp, FeNN::Common::CSR::SLAVE_BUFFER_OVERFLOW_COUNT);
                c.sw(*STmp, FeNN::Common::Reg::X0, bufferOverflowCountPtr);
            }
            // Store event count and; min and max IDs
            c.sw(*SNumEvents, FeNN::Common::Reg::X0, eventCounterPtr);
            c.sw(*SMinEventID, FeNN::Common::Reg::X0, minIDPtr);
            c.sw(*SMaxEventID, FeNN::Common::Reg::X0, maxIDPtr);
        });

    // Dump to coe file
    FeNN::Common::AppUtils::dumpCOE("event_camera.coe", code);
    
    // Load input sequence
    const auto spikeInjectData = FeNN::Common::AppUtils::loadBinaryData<uint32_t>("courtyard.bin");

    if(device) {
        // Allocate vector with data for all cores
        CoreData coreData(numCores);
        
        ::Common::Barrier barrier(numCores);
        // Loop through cores
        for(uint32_t i = 0; i < numCores; i++) {
            // Create thread
            std::get<1>(coreData[i]) = std::thread(
                deviceThread, std::cref(code), std::cref(scalarInitData),
                i, eventCounterPtr, minIDPtr, maxIDPtr, bufferOverflowCountPtr, readyFlagPtr, 
                std::ref(std::get<0>(coreData[i])), std::ref(barrier));
            
            // Name thread
            ::Common::Utils::setThreadName(std::get<1>(coreData[i]), "Core " + std::to_string(i));
        }

        // Join all threads
        for(auto &c : coreData) {
            std::get<1>(c).join();
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
