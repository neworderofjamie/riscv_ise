#include "backend/memory_allocator.h"

// Standard C includes
#include <cstdint>

// Plog includes
#include <plog/Log.h>

// Common includes
#include "common/dma_buffer.h"
#include "common/utils.h"

//----------------------------------------------------------------------------
// MemoryAllocator
//----------------------------------------------------------------------------
size_t MemoryAllocator::allocate(size_t sizeBytes)
{
    assert(m_HighWaterBytes % m_AlignementBytes == 0);

    // Pad variable size to alignement bytes
    const size_t varSize = padSize(sizeBytes, m_AlignementBytes);

    // Update highwater and check against size
    const size_t newHighWaterBytes = m_HighWaterBytes + varSize;
    if(newHighWaterBytes > m_SizeBytes) {
        throw std::runtime_error("Memory exceeded");
    }

    LOGD << "Allocating " << varSize << " bytes of " << m_Context << " starting at " << m_HighWaterBytes << " bytes";    

    // Stash old high-water mark, update and return
    const size_t address = m_HighWaterBytes;
    m_HighWaterBytes = newHighWaterBytes;
    return address;
}

//----------------------------------------------------------------------------
// DMABufferAllocator
//----------------------------------------------------------------------------
// **THINK** do we want to vector align DMA buffer and remove hardware alignement
DMABufferAllocator::DMABufferAllocator(const DMABuffer &dmaBuffer)
:   MemoryAllocator(dmaBuffer.getSize(), 4, "DMA buffer")
{
}