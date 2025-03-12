#include "compiler/memory_allocator.h"

// Standard C includes
#include <cstdint>

// Common includes
#include "common/utils.h"

// Compiler includes
#include "compiler/event_container.h"
#include "compiler/variable.h"

//----------------------------------------------------------------------------
// MemoryAllocator
//----------------------------------------------------------------------------
URAMAddress MemoryAllocator::allocate(const Variable &variable)
{
    assert(m_URAMHighWaterBytes % 64 == 0);

    // Pad variable size to 64 bytes
    const size_t varSize = padSize(variable.getShape().getFlattenedSize() 
                                   * variable.getType().getSize(0), 64);

    // Update highwater and check against size
    const size_t newHighWaterBytes = m_URAMHighWaterBytes + varSize;
    if(newHighWaterBytes > m_URAMSizeBytes) {
        throw std::runtime_error("URAM exceeded");
    }

    // Build type-safe address wrapper and return
    URAMAddress address(m_URAMHighWaterBytes);
    m_URAMHighWaterBytes = newHighWaterBytes;
    return address;
}
//----------------------------------------------------------------------------
BRAMAddress MemoryAllocator::allocate(const EventContainer &eventContainer)
{
    assert(m_BRAMHighWaterBytes % 4 == 0);

    // Events are stored 1 bit per neuron in 32-bit words
    const size_t numEventWords = ceilDivide(eventContainer.getShape().getNumNeurons(), 32);

    // Update highwater and check against size
    const size_t newHighWaterBytes = m_BRAMHighWaterBytes + (numEventWords * 4);
    if(newHighWaterBytes > m_BRAMSizeBytes) {
        throw std::runtime_error("BRAM exceeded");
    }

    // Build type-safe address wrapper and return
    BRAMAddress address(m_BRAMHighWaterBytes);
    m_BRAMHighWaterBytes = newHighWaterBytes;
    return address;
}
