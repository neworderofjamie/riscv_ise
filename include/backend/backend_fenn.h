#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Standard C includes
#include <cstdint>

// GeNN includes
#include "type.h"

// Backend include
#include "backend/memory_allocator.h"
#include "backend/model.h"

// Forward declarations
class ProcessGroup;

//----------------------------------------------------------------------------
// StateBase
//----------------------------------------------------------------------------
//! Base class for runtime state objects created by backend
class StateBase
{
public:
    virtual ~StateBase()
    {
    }

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual void setInstructions(const std::vector<uint32_t> &instructions) = 0;
    virtual void run() = 0;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getBRAMAllocator() const{ return m_BRAMAllocator; }
    auto &getBRAMAllocator(){ return m_BRAMAllocator; }

    const auto &getURAMAllocator() const{ return m_URAMAllocator; }
    auto &getURAMAllocator(){ return m_URAMAllocator; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    BRAMAllocator m_BRAMAllocator;
    URAMAllocator m_URAMAllocator;
};

//----------------------------------------------------------------------------
// ArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays created by backend
//! **NOTE** this is a temporary, simplified version of GeNN's ArrayBase
class ArrayBase
{
public:
    virtual ~ArrayBase()
    {
    }

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Allocate array
    virtual void allocate(size_t count) = 0;

    //! Free array
    virtual void free() = 0;

    //! Copy entire array to device
    virtual void pushToDevice() = 0;

    //! Copy entire array from device
    virtual void pullFromDevice() = 0;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const GeNN::Type::ResolvedType &getType() const{ return m_Type; }
    size_t getCount() const{ return m_Count; };
    size_t getSizeBytes() const{ return m_Count * m_Type.getValue().size; };

    //! Get array host pointer
    uint8_t *getHostPointer() const{ return m_HostPointer; }

    template<typename T>
    T *getHostPointer() const{ return reinterpret_cast<T*>(m_HostPointer); }

    //! Memset the host pointer
    void memsetHostPointer(int value);

protected:
    ArrayBase(const GeNN::Type::ResolvedType &type, size_t count)
    :   m_Type(type), m_Count(count), m_HostPointer(nullptr)
    {
    }

    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------    
    void setCount(size_t count) { m_Count = count; }
    void setHostPointer(uint8_t *hostPointer) { m_HostPointer = hostPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    GeNN::Type::ResolvedType m_Type;
    size_t m_Count;

    uint8_t *m_HostPointer;
};

//----------------------------------------------------------------------------
// URAMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in FeNN's URAM
class URAMArrayBase : public ArrayBase
{
public:
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    uint32_t getURAMPointer() const{ return m_URAMPointer.value(); }

protected:
    URAMArrayBase(const GeNN::Type::ResolvedType &type, size_t count)
    :   ArrayBase(type, count)
    {
        if(type.getSize(0) != 2) {
            throw std::runtime_error("Only 16-bit types can be stored in URAM arrays");
        }
    }

    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    void setURAMPointer(std::optional<uint32_t> uramPointer){ m_URAMPointer = uramPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::optional<uint32_t> m_URAMPointer;
};

//----------------------------------------------------------------------------
// BRAMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in FeNN's BRAM
class BRAMArrayBase : public ArrayBase
{
public:
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    uint32_t getBRAMPointer() const{ return m_BRAMPointer.value(); }

protected:
    using ArrayBase::ArrayBase;

    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    void setBRAMPointer(std::optional<uint32_t> uramPointer){ m_BRAMPointer = uramPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::optional<uint32_t> m_BRAMPointer;
};

//----------------------------------------------------------------------------
// BackendFeNN
//----------------------------------------------------------------------------
//! Base class for FeNN backends
//! **NOTE** this is a temporary, simplified version of GeNN's BackendBase
class BackendFeNN
{
public:
    virtual ~BackendFeNN()
    {}

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<ArrayBase> createURAMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                       StateBase *state) const = 0;
    virtual std::unique_ptr<ArrayBase> createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                       StateBase *state) const = 0;

    virtual std::unique_ptr<StateBase> createState() const = 0;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    std::vector<uint32_t> generateSimulationKernel(std::shared_ptr<const ProcessGroup> synapseProcessGroup, 
                                                   std::shared_ptr<const ProcessGroup> neuronProcessGroup,
                                                   uint32_t numTimesteps, bool simulate,
                                                   const Model &model) const;
};