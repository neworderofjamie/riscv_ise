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
void simRouterThread(const std::vector<uint32_t> &code, const std::vector<uint8_t> &scalarInitData, uint32_t core,
                     SharedBusSim &sharedBus, uint32_t bitfieldPtr, uint32_t eventIDBasePtr, uint32_t spikeQueuePtr)
{
    // Create RISC-V core with instruction and scalar data
    RISCV riscV;
    riscV.setInstructions(code);
    riscV.getScalarDataMemory().setData(scalarInitData);
    
    auto *scalarData = reinterpret_cast<uint32_t*>(riscV.getScalarDataMemory().getData());

    // Write random bitfield to memory
    std::random_device r;
    scalarData[bitfieldPtr / 4] = r();

    // Generate event ID base from core index
    scalarData[eventIDBasePtr / 4] = (core << 14);

    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);
        
    // Create simulated DMA controller
    RouterSim router(sharedBus, riscV.getScalarDataMemory());
    riscV.setRouter(&router);
     
    // Run!
    if(!riscV.run()) {
        FAIL();
    }

    // Check results
    checkBuffer(bufferData, transferHalfWords);
}
}

//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(Router, Test) {
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t bitfieldPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t eventIDBasePtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t spikeQueueEndPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t spikeQueuePtr = AppUtils::allocateScalarAndZero(4 * 32, scalarInitData);

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

            c.
        });

    
}