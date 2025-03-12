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
// URAMAddress
//----------------------------------------------------------------------------
//! Thin wrapper to provide type safety
class URAMAddress
{
public:
    URAMAddress(uint32_t address) : m_Address(address)
    {
    }

    uint32_t get() const{ return m_Address; }

private:
    uint32_t m_Address;
};


//----------------------------------------------------------------------------
// BRAMAddress
//----------------------------------------------------------------------------
//! Thin wrapper to provide type safety
class BRAMAddress
{
public:
    BRAMAddress(uint32_t address) : m_Address(address)
    {
    }

    uint32_t get() const{ return m_Address; }

private:
    uint32_t m_Address;
};

//----------------------------------------------------------------------------
// MemoryAllocator
//----------------------------------------------------------------------------
class MemoryAllocator
{
public:
    MemoryAllocator(uint32_t bramSizeBytes = 128 * 1024,
                    uint32_t uramSizeBytes = 8 * 4096 * 2 * 8)
    :   m_BRAMSizeBytes(bramSizeBytes), m_URAMSizeBytes(uramSizeBytes),
        m_BRAMHighWaterBytes(0), m_URAMHighWaterBytes(0)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    URAMAddress allocate(const Variable &variable);
    BRAMAddress allocate(const EventContainer &eventContainer);

    size_t getFreeURAMBytes() const{ return m_URAMSizeBytes - m_URAMHighWaterBytes; }
    size_t getFreeBRAMBytes() const{ return m_BRAMSizeBytes - m_BRAMHighWaterBytes; };

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    uint32_t m_BRAMSizeBytes;
    uint32_t m_URAMSizeBytes;
    uint32_t m_BRAMHighWaterBytes;
    uint32_t m_URAMHighWaterBytes;
};

using URAMAllocations = std::unordered_map<const ModelComponent*, URAMAddress>;
using BRAMAllocations = std::unordered_map<const ModelComponent*, BRAMAddress>;

//----------------------------------------------------------------------------
// MemoryAllocatorVisitor
//----------------------------------------------------------------------------
class MemoryAllocatorVisitor : public ModelComponentVisitor
{
public:
    MemoryAllocatorVisitor(MemoryAllocator &memoryAllocator);

    //------------------------------------------------------------------------
    // ModelComponentVisitor virtuals
    //------------------------------------------------------------------------
    virtual void visit(const EventContainer &eventContainer) final;
    virtual void visit(const Variable &variable) final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getURAMAllocations() const{ return m_URAMAllocations; }
    const auto &getBRAMAllocations() const{ return m_BRAMAllocations; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::reference_wrapper<MemoryAllocator> m_MemoryAllocator;
    URAMAllocations m_URAMAllocations;
    BRAMAllocations m_BRAMAllocations;
};