#pragma once

// Standard C++ includes
#include <functional>
#include <string>
#include <unordered_map>

// Standard C includes
#include <cstddef>
#include <cstdint>

// Backend includes
#include "fenn/backend/backend_export.h"


//----------------------------------------------------------------------------
// MemoryAllocator
//----------------------------------------------------------------------------
class MemoryAllocator
{
public:
    MemoryAllocator(size_t sizeBytes, size_t alignementBytes, const std::string &context)
    :   m_SizeBytes(sizeBytes), m_AlignementBytes(alignementBytes), m_HighWaterBytes(0), m_Context(context)
    {}
    MemoryAllocator(const MemoryAllocator&) = delete;

    size_t allocate(size_t sizeBytes);

    size_t getFreeBytes() const{ return m_SizeBytes - m_HighWaterBytes; }

private:
    size_t m_SizeBytes;
    size_t m_AlignementBytes;
    size_t m_HighWaterBytes;
    std::string m_Context;
};

//----------------------------------------------------------------------------
// BRAMAllocator
//----------------------------------------------------------------------------
class BRAMAllocator : public MemoryAllocator
{
public:
    BRAMAllocator() : MemoryAllocator(128 * 1024, 4, "BRAM")
    {}
};

//----------------------------------------------------------------------------
// URAMAllocator
//----------------------------------------------------------------------------
class URAMAllocator : public MemoryAllocator
{
public:
    URAMAllocator() : MemoryAllocator(8 * 4096 * 2 * 8, 64, "URAM")
    {}
};

//----------------------------------------------------------------------------
// LLMAllocator
//----------------------------------------------------------------------------
class LLMAllocator : public MemoryAllocator
{
public:
    LLMAllocator() : MemoryAllocator(32 * 1024 * 2, 64, "LLM")
    {}
};

//----------------------------------------------------------------------------
// DMABufferAllocator
//----------------------------------------------------------------------------
class BACKEND_EXPORT DMABufferAllocator : public MemoryAllocator
{
public:
    DMABufferAllocator(size_t bufferSize) : MemoryAllocator(bufferSize, 64, "DMA buffer")
    {}
};