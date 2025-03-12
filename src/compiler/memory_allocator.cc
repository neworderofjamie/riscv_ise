#include "compiler/memory_allocator.h"

// Standard C includes
#include <cstdint>

// Plog includes
#include <plog/Log.h>

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

    LOGD << "Allocating " << varSize << " bytes of URAM starting at " << m_URAMHighWaterBytes << " bytes for variable '" << variable.getName() << "'";    

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

    LOGD << "Allocating " << numEventWords * 4 << " bytes of BRAM starting at " << m_BRAMHighWaterBytes << " bytes for event container '" << eventContainer.getName() << "'";

    // Build type-safe address wrapper and return
    BRAMAddress address(m_BRAMHighWaterBytes);
    m_BRAMHighWaterBytes = newHighWaterBytes;
    return address;
}

//----------------------------------------------------------------------------
// MemoryAllocatorVisitor
//----------------------------------------------------------------------------
MemoryAllocatorVisitor::MemoryAllocatorVisitor(MemoryAllocator &memoryAllocator)
:   m_MemoryAllocator(memoryAllocator)
{

}
//------------------------------------------------------------------------
void MemoryAllocatorVisitor::visit(const EventContainer &eventContainer)
{
    if(!m_BRAMAllocations.try_emplace(&eventContainer, m_MemoryAllocator.get().allocate(eventContainer)).second) {
        throw std::runtime_error("BRAM already allocated for event container");
    }
}
//----------------------------------------------------------------------------
void MemoryAllocatorVisitor::visit(const Variable &variable)
{
    if(!m_URAMAllocations.try_emplace(&variable, m_MemoryAllocator.get().allocate(variable)).second) {
        throw std::runtime_error("URAM already allocated for variable");
    }
}