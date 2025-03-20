#pragma once

// Standard C++ includes
#include <functional>
#include <unordered_map>

// Standard C includes
#include <cstddef>
#include <cstdint>

// Compiler includes
#include "compiler/model_component.h"

// Forward declarations
class DMABuffer;
class EventContainer;
class Variable;

//----------------------------------------------------------------------------
// MemoryAllocator
//----------------------------------------------------------------------------
class MemoryAllocator
{
public:
    MemoryAllocator(size_t sizeBytes, size_t alignementBytes)
    :   m_SizeBytes(sizeBytes), m_AlignementBytes(alignementBytes), m_HighWaterBytes(0)
    {}
    MemoryAllocator(const MemoryAllocator&) = delete;

    size_t allocate(size_t sizeBytes);

    size_t getFreeBytes() const{ return m_SizeBytes - m_HighWaterBytes; }

private:
    size_t m_SizeBytes;
    size_t m_AlignementBytes;
    size_t m_HighWaterBytes;
};

//----------------------------------------------------------------------------
// BRAMAllocator
//----------------------------------------------------------------------------
class BRAMAllocator : public MemoryAllocator
{
public:
    BRAMAllocator() : MemoryAllocator(128 * 1024, 4)
    {}
};

//----------------------------------------------------------------------------
// URAMAllocator
//----------------------------------------------------------------------------
class URAMAllocator : public MemoryAllocator
{
public:
    URAMAllocator() : MemoryAllocator(8 * 4096 * 2 * 8, 64)
    {}
};

//----------------------------------------------------------------------------
// DMABufferAllocator
//----------------------------------------------------------------------------
class DMABufferAllocator : public MemoryAllocator
{
public:
    DMABufferAllocator(const DMABuffer &dmaBuffer);
};