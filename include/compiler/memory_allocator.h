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
class EventContainer;
class Variable;

//----------------------------------------------------------------------------
// MemoryAllocator
//----------------------------------------------------------------------------
class MemoryAllocator
{
public:
    MemoryAllocator(uint32_t sizeBytes, uint32_t alignementBytes)
    :   m_SizeBytes(sizeBytes), m_AlignementBytes(alignementBytes), m_HighWaterBytes(0)
    {}
    MemoryAllocator(const MemoryAllocator&) = delete;

    uint32_t allocate(uint32_t sizeBytes);
    uint32_t allocate(uint32_t elementSize, uint32_t count){ return allocate(elementSize * count); }

    uint32_t getFreeBytes() const{ return m_SizeBytes - m_HighWaterBytes; }

private:
    uint32_t m_SizeBytes;
    uint32_t m_AlignementBytes;
    uint32_t m_HighWaterBytes;
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