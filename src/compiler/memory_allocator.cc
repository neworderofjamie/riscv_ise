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
uint32_t MemoryAllocator::allocate(uint32_t sizeBytes)
{
    assert(m_HighWaterBytes % m_AlignementBytes == 0);

    // Pad variable size to alignement bytes
    const size_t varSize = padSize(sizeBytes, m_AlignementBytes);

    // Update highwater and check against size
    const size_t newHighWaterBytes = m_HighWaterBytes + varSize;
    if(newHighWaterBytes > m_SizeBytes) {
        throw std::runtime_error("Memory exceeded");
    }

    LOGD << "Allocating " << varSize << " bytes of memorty starting at " << m_HighWaterBytes << " bytes";    

    // Stash old high-water mark, update and return
    const uint32_t address = m_HighWaterBytes;
    m_HighWaterBytes = newHighWaterBytes;
    return address;
}
