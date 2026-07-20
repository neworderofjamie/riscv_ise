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
constexpr uint32_t spikeArrayPtr = 32 * 4096;
constexpr size_t numCores = 1;

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

void simEventInjectorThread(SharedBusSim &sharedBus, uint32_t coreID, const std::vector<uint32_t> &data)
{
    // Create event injector
    EventInjectorSim eventInjector(sharedBus, data, coreID);

    // Keep ticking event injector until it runs out of data
    while(eventInjector.tick());
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
            PLOGW << missingCorrectEvents.size() << " events missing from core output at time " << timestep;
        }

        if (!missingOutputEvents.empty()) {
            PLOGE << missingOutputEvents.size() << " events in output which weren't in input at time " << timestep;
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
    
    // If downsampling, allocate downsample buffer
    std::optional<uint32_t> downsampleIBuffer;
    if (downsampleShift > 0) {
        downsampleIBuffer = AppUtils::allocateScalarAndZero(downsampleResolution * downsampleResolution, scalarInitData);
    }

    // Allocate remaining scalar memory for event storage
    const uint32_t eventMemoryPtr = AppUtils::allocateScalarAndZero((4096 * 32) - scalarInitData.size(), scalarInitData);

    // If downsampling, allocate state variable for downsampling neurons
    std::optional<uint32_t> downsampleVBuffer;
    if (downsampleShift > 0) {
        downsampleVBuffer = AppUtils::allocateVectorAndZero(downsampleResolution * downsampleResolution, vectorInitData);
    }

    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, 
            ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeEnd);
            ALLOCATE_SCALAR(STimeMarker);
            ALLOCATE_SCALAR(SEventOutputBuffer);
            

            // Start time at 0
            c.li(*STime, 0);
            c.li(*STimeEnd, 88);

            // Bit to mark timestamps with
            c.li(*STimeMarker, 1 << 31);

            // Load spike memory pointers
            c.li(*SEventOutputBuffer, eventMemoryPtr);

            auto timeLoop = c.L();
            {
                ALLOCATE_SCALAR(SLoopStartCycleLow);
                ALLOCATE_SCALAR(SLoopStartCycleHigh);

                // Wait for all events to be communicated
                // **NOTE** this is only necessary in simulation to synchronise event injector
                if(!device) {
                    AssemblerUtils::generateRouterBarrier(c, scalarRegisterAllocator, numCores + 1);
                }

                // Read cycle count at start of loop
                c.csrr(*SLoopStartCycleLow, CSR::MCYCLE);
                c.csrr(*SLoopStartCycleHigh, CSR::MCYCLEH);

                // Swap router buffers
                c.csrwi(CSR::SLAVE_SWAP_BUFFER, 1);

                {
                    Label eventLoopEnd;

                    // Get start and end of event buffer
                    ALLOCATE_SCALAR(SRouterEventBuffer);
                    ALLOCATE_SCALAR(SRouterEventBufferEnd);
                    c.csrr(*SRouterEventBuffer, CSR::SLAVE_EVENT_START_ADDRESS);
                    c.csrr(*SRouterEventBufferEnd, CSR::SLAVE_EVENT_END_ADDRESS);
                    
                    // If there aren't any events goto end
                    c.beq(*SRouterEventBuffer, *SRouterEventBufferEnd, eventLoopEnd);

                    // If we're not downsampling, write time with top bit set to spike memory
                    if(downsampleShift == 0) {
                        ALLOCATE_SCALAR(STmp);
                        c.or_(*STmp, *STime, *STimeMarker);
                        c.sw(*STmp, *SEventOutputBuffer);

                        // Advance spike memory pointer
                        c.addi(*SEventOutputBuffer, *SEventOutputBuffer, 4);
                    }

                    // While (spikeBuffer != spikeBufferEnd
                    auto eventLoop = c.L();
                    
                    {
                        ScalarRegisterAllocator::RegisterPtr SPolarityMask;
                        ScalarRegisterAllocator::RegisterPtr SDownsampleResolution;
                        ScalarRegisterAllocator::RegisterPtr SDownsampleBuffer;
                        if (downsampleShift > 0) {
                            SPolarityMask = scalarRegisterAllocator.getRegister("SPolarityMask");
                            SDownsampleResolution = scalarRegisterAllocator.getRegister("SDownsampleResolution");
                            SDownsampleBuffer = scalarRegisterAllocator.getRegister("SDownsampleBuffer");

                            // Bit mask to extract polarity with
                            c.li(*SPolarityMask, 1 << 18);
                            c.li(*SDownsampleResolution, downsampleResolution);
                            c.li(*SDownsampleBuffer, downsampleIBuffer.value());
                        }

                        c.beq(*SRouterEventBuffer, *SRouterEventBufferEnd, eventLoopEnd);
                        {
                            ALLOCATE_SCALAR(SEvent);

                            // Load spike from buffer and incrememnt pointer
                            c.lw(*SEvent, *SRouterEventBuffer);
                            c.addi(*SRouterEventBuffer, *SRouterEventBuffer, 4);

                            // If we're downsampling
                            if (downsampleShift > 0) {
                                ALLOCATE_SCALAR(SX);
                                ALLOCATE_SCALAR(SCoord);
                                ALLOCATE_SCALAR(SP);
                                ALLOCATE_SCALAR(SDownsampleIntegrator);

                                // Extract downsampled y from bottom of event
                                c.srli(*SCoord, *SEvent, downsampleShift);
                                c.andi(*SCoord, *SCoord, downsampleMask);
                            
                                // Extract downsampled x from above
                                c.srli(*SX, *SEvent, downsampleShift + 9);
                                c.andi(*SX, *SX, downsampleMask);
                            
                                // Calculate (Y * downsampleResolution) + X
                                c.mul(*SCoord, *SCoord, *SDownsampleResolution);
                                c.add(*SCoord, *SCoord, *SX);
                                c.add(*SCoord, *SCoord, *SDownsampleBuffer);

                                // Extract polarity from above that
                                c.and_(*SP, *SEvent, *SPolarityMask);

                                // Load current downsampled value
                                c.lb(*SDownsampleIntegrator, *SCoord);

                                // Shift down by 17 so it's either 0 or 2
                                c.srli(*SP, *SP, 17);

                                // Subtract 1 so it's either -1  or 1
                                c.addi(*SP, *SP, -1);

                                // Add to integrator
                                c.add(*SDownsampleIntegrator, *SDownsampleIntegrator, *SP);

                                // Store
                                c.sb(*SDownsampleIntegrator, *SCoord);
                            }
                            // Otherwise
                            else {
                                // Write event directly to spike memory
                                c.sw(*SEvent, *SEventOutputBuffer);

                                // Advance spike memory pointer and goto end if end of memory reached
                                c.addi(*SEventOutputBuffer, *SEventOutputBuffer, 4);
                            }

                            // Next event
                            c.j_(eventLoop);
                        }
                        c.L(eventLoopEnd);
                    }
                }

                if (downsampleShift > 0) {
                    ALLOCATE_SCALAR(SVBuffer);
                    ALLOCATE_SCALAR(SIBuffer);
                    ALLOCATE_SCALAR(SOne);
                    ALLOCATE_VECTOR(VAlpha);
                    ALLOCATE_VECTOR(VZero);
                    
                    c.li(*SVBuffer, downsampleVBuffer.value());
                    c.li(*SIBuffer, downsampleIBuffer.value());
                    c.li(*SOne, 1);
                    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / 20.0), 12));
                    c.vlui(*VZero, 0);

                    AssemblerUtils::unrollVectorLoopBody(
                        c, scalarRegisterAllocator, downsampleResolution * downsampleResolution, 4, *SVBuffer,
                        [&scalarRegisterAllocator, &vectorRegisterAllocator, 
                         SOne, SIBuffer, SVBuffer, VAlpha]
                        (CodeGenerator &c, uint32_t r, bool, ScalarRegisterAllocator::RegisterPtr maskReg)
                        {
                            // Register allocation
                            ALLOCATE_VECTOR(VV);
                            ALLOCATE_VECTOR(VISyn);
                            ALLOCATE_SCALAR(SSpikeOut);
                         
                            // Zero ISyn
                            // **OPTIMISE** process is very slow - 4 instructions per-neuron. If we had an instruction 
                            // that filled all lanes with sign-extended bytes - would reduce by factor 4 without muxes
                            c.vlui(*VISyn, 0);
                            {
                                // Loop through bytes of downsample buffer
                                ALLOCATE_SCALAR(SI);
                                ALLOCATE_SCALAR(SMask);
                                ALLOCATE_VECTOR(VTmp);
                                for (int i = 0; i < 32; i++) {
                                    // Load I byte
                                    c.lb(*SI, *SIBuffer, i);

                                    // Build mask
                                    c.slli(*SMask, *SOne, i);
                                    
                                    // Fill vector register
                                    c.vfill(*VTmp, *SI);
                                    
                                    // Fill lane of ISyn with masked value
                                    c.vsel(*VISyn, *SMask, *VTmp);
                                }

                                // Zero buffer
                                for (int i = 0; i < 8; i++) {
                                    c.sw(Reg::X0, *SIBuffer, i * 4);
                                }
                            }

                            // Load voltage and isyn
                            c.vloadv(*VV, *SVBuffer, 64 * r);
                            
                            // VV *= VAlpha
                            c.vmul(12, *VV, *VV, *VAlpha);

                            // VV += VISyn
                            c.vadd_s(*VV, *VV, *VISyn);

                            // VISyn = 0
                            c.vlui(*VISyn, 0);

                            // SSpikeOut = VV >= VThresh && !SRefractory
                            /*c.vtge(*SSpikeOut, *VV, *VThresh);

                            // *SSpikeBuffer = SSpikeOut
                            c.sw(*SSpikeOut, *SSpikeBuffer, 4 * r);
                            {
                                // VTemp = V - VThresh
                                ALLOCATE_VECTOR(VTemp);
                                c.vadd(*VTemp, *VV, *VMinusThresh);

                                // VV = SSpikeOut ? VReset : VV
                                c.vsel(*VV, *SSpikeOut, *VTemp);
                            }*/

                            // Store V
                            c.vstore(*VV, *SVBuffer, 64 * r);
                        },
                        [SIBuffer, /*SSpikeBuffer, */SVBuffer]
                        (CodeGenerator &c, uint32_t numUnrolls)
                        {
                            c.addi(*SIBuffer, *SIBuffer, 32 * numUnrolls);
                            c.addi(*SVBuffer, *SVBuffer, 64 * numUnrolls);
                            //c.addi(*SSpikeBuffer, *SSpikeBuffer, 4 * numUnrolls); 
                        });
                }
                

                // Wait until 1ms of clock cycles has elapsed since start of loop
                if(device) {
                    AssemblerUtils::generateWaitElapsedCycles(c, scalarRegisterAllocator, 
                                                              *SLoopStartCycleLow, *SLoopStartCycleHigh, 
                                                              clockSpeedMhz * 1000);
                }

                // Increment time
                c.addi(*STime, *STime, 1);
                c.bne(*STime, *STimeEnd, timeLoop);
            }
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
        SharedBusSim sharedBus(numCores + 1);
    
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
    }
    return 0;

}
