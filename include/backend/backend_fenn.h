#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Standard C includes
#include <cstdint>

// GeNN includes
#include "type.h"

// Compiler ioncludes
#include "compiler/process_fields.h"

// Forward declarations
class ProcessGroup;

class StateBase
{
public:
    virtual ~StateBase()
    {
    }
};

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

class URAMArrayBase : public ArrayBase
{
public:
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    uint32_t getURAMPointer() const{ return m_URAMPointer.value(); }

protected:
    void setURAMPointer(std::optional<uint32_t> uramPointer){ m_URAMPointer = uramPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::optional<uint32_t> m_URAMPointer;
};

class BRAMArrayBase : public ArrayBase
{
public:
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    uint32_t getBRAMPointer() const{ return m_BRAMPointer.value(); }

protected:
    void setBRAMPointer(std::optional<uint32_t> uramPointer){ m_BRAMPointer = uramPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::optional<uint32_t> m_BRAMPointer;
};

class BackendFeNN
{
public:
    virtual ~BackendFeNN()
    {}

    // Declared virtuals
    virtual std::unique_ptr<ArrayBase> createURAMArray(StateBase *state, const GeNN::Type::ResolvedType &type, size_t count) const = 0;
    virtual std::unique_ptr<ArrayBase> createBRAMArray(StateBase *state, const GeNN::Type::ResolvedType &type, size_t count) const = 0;

    virtual std::unique_ptr<StateBase> createState() const = 0;

    // Public API
    std::vector<uint32_t> generateSimulationKernel(std::shared_ptr<const ProcessGroup> synapseProcessGroup, 
                                                   std::shared_ptr<const ProcessGroup> neuronProcessGroup,
                                                   const ProcessFields &processFields,
                                                   uint32_t numTimesteps, bool simulate) const;

};