// Standard C++ includes
#include <algorithm>
#include <fstream>
#include <numeric>
#include <random>
#include <tuple>
#include <thread>

// PLOG includes
#include <plog/Severity.h>

// Google test includes
#include "gtest/gtest.h"

// RISC-V common includes
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

// SoC test includes
#include "fixtures.h"

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
void simRouterThread(const std::vector<uint32_t> &code, const std::vector<uint8_t> &scalarInitData,
                     SharedBusSim &sharedBus, uint32_t coreEventIDBase, uint32_t spikeBitfield,
                     uint32_t bitfieldPtr, uint32_t eventIDBasePtr, uint32_t spikeQueueEndPtr, 
                     uint32_t spikeQueuePtr, std::vector<uint32_t> &receivedEvents)
{
    // Create RISC-V core with instruction and scalar data
    RISCV riscV;
    riscV.setInstructions(code);
    riscV.getScalarDataMemory().setData(scalarInitData);
    
    // Write bitfield and event ID base to memory
    auto *scalarWordData = reinterpret_cast<uint32_t*>(riscV.getScalarDataMemory().getData());
    scalarWordData[bitfieldPtr / 4] = spikeBitfield;
    scalarWordData[eventIDBasePtr / 4] = coreEventIDBase;

    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);
        
    // Create simulated DMA controller
    RouterSim router(sharedBus, riscV.getScalarDataMemory());
    riscV.setRouter(&router);
     
    // Run!
    if(!riscV.run()) {
        FAIL();
    }

    // Copy spikes received into vecto
    const uint32_t spikeQueueEnd = scalarWordData[spikeQueueEndPtr / 4];
    std::copy(scalarWordData + (spikeQueuePtr / 4), 
              scalarWordData + (spikeQueueEnd / 4),
              std::back_inserter(receivedEvents));
}
}

//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(Router, Test) {
    const uint32_t numCores = 2;

    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t bitfieldPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t eventIDBasePtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t spikeQueueEndPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t spikeQueuePtr = AppUtils::allocateScalarAndZero(4 * 64, scalarInitData);

    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        true/*!std::get<2>(GetParam())*/, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator&, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Load event ID base
            {
                ALLOCATE_SCALAR(STmp);
                c.lw(*STmp, Reg::X0, eventIDBasePtr);
                c.csrw(CSR::MASTER_EVENT_ID_BASE, *STmp);
            }

            // Configure spike queue address
            c.csrwi(CSR::SLAVE_EVENT_ADDRESS, spikeQueuePtr);

            // Wait on barrier
            AssemblerUtils::generateRouterBarrier(c, scalarRegisterAllocator, 2);

            // Send spikes
            {
                ALLOCATE_SCALAR(STmp);
                c.lw(*STmp, Reg::X0, bitfieldPtr);
                c.csrw(CSR::MASTER_EVENT_BITFIELD, *STmp);
            }

            // Wait on barrier
            AssemblerUtils::generateRouterBarrier(c, scalarRegisterAllocator, 2);

            // Write end write address to memory
            {
                ALLOCATE_SCALAR(STmp);
                c.csrr(*STmp, CSR::SLAVE_EVENT_ADDRESS);
                c.sw(*STmp, Reg::X0, spikeQueueEndPtr);
            }
        });

        // Simulation
        if(true) {
            // Create simulated shared bus to connect the cores
            SharedBusSim sharedBus(numCores);

            // Allocate vector with data for all cores
            std::vector<std::tuple<uint32_t, std::vector<uint32_t>, std::thread>> coreData(numCores);

            // Loop through cores
            std::random_device d;
            for(uint32_t i = 0; i < numCores; i++) {
                // Generate spike bitfield for this core
                std::get<0>(coreData[i]) = d();

                // Create thread
                std::get<2>(coreData[i]) = std::thread(
                    simRouterThread, std::cref(code), std::cref(scalarInitData),
                    std::ref(sharedBus), i << 14, std::get<0>(coreData[i]),
                    bitfieldPtr, eventIDBasePtr, spikeQueueEndPtr, 
                    spikeQueuePtr, std::ref(std::get<1>(coreData[i])));
                
                // Name thread
                setThreadName(std::get<2>(coreData[i]), "Core " + std::to_string(i));
            }

            // Join all threads
            for(auto &c : coreData) {
                std::get<2>(c).join();
            }

            // Check all cores have received the same data
            // **NOTE** may need to sort first on real hardware
            const auto &core0Data = std::get<1>(coreData[0]);
            for(uint32_t i = 1; i < numCores; i++) {
                const auto &coreIData = std::get<1>(coreData[i]);
                ASSERT_TRUE(std::equal(core0Data.cbegin(), core0Data.cend(),
                                       coreIData.cbegin(), coreIData.cend()));
            }

            // Make a copy of each core's bitfield
            std::vector<uint32_t> spikeBitfieldCopy;
            std::transform(coreData.cbegin(), coreData.cend(), std::back_inserter(spikeBitfieldCopy),
                           [](const auto &c){ return std::get<0>(c); });

            // Loop through events received by one core
            for(uint32_t s : std::get<1>(coreData[0])) {
                // Split event into core and neuron
                const uint32_t core = s >> 14;
                const uint32_t neuron = s & ((1 << 14) - 1);
                const uint32_t neuronBit = 1 << neuron;

                // Check bit is still set in core's bitmask
                ASSERT_NE(spikeBitfieldCopy[core] & neuronBit, 0);

                // Clear BIT
                spikeBitfieldCopy[core] &= ~neuronBit;
            }

            // Check all bits have been cleared i.e. received spikes match bitfield
            for(uint32_t s : spikeBitfieldCopy) {
                ASSERT_EQ(s, 0);
            }
        }
}