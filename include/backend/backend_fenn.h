#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Standard C includes
#include <cstdint>
#include <cstring>

// GeNN includes
#include "type.h"

// Backend include
#include "backend/backend_export.h"
#include "backend/memory_allocator.h"
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
    StateBase();
    virtual ~StateBase() = default;
    StateBase(const StateBase &) = delete;

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

    size_t getReadyFlagPointer() const { return m_ReadyFlagPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    BRAMAllocator m_BRAMAllocator;
    URAMAllocator m_URAMAllocator;
    size_t m_ReadyFlagPointer;
};

//----------------------------------------------------------------------------
// ArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays created by backend
//! **NOTE** this is a temporary, simplified version of GeNN's ArrayBase
class BACKEND_EXPORT ArrayBase
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

    //! Serialise backend-specific device object to uint32_t
    virtual void serialiseDeviceObject(std::vector<std::byte> &bytes) const = 0;

    //! Memset the host pointer
    virtual void memsetHostPointer(int value);

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
// IFieldArray
//----------------------------------------------------------------------------
//! Interface for object, probably backed by some sort of array,
//! that anages in-device memory fields structure
class IFieldArray
{
public:
    virtual ~IFieldArray() = default;

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Sets field at offset to point to array
    virtual void setFieldArray(uint32_t fieldOffset, const ArrayBase *array) = 0;

    //! Copy field data to device
    virtual void pushFieldsToDevice() = 0;
};

//----------------------------------------------------------------------------
// URAMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in FeNN's URAM
class BACKEND_EXPORT URAMArrayBase : public ArrayBase
{
public:
    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Serialise backend-specific device object to uint32_t
    virtual void serialiseDeviceObject(std::vector<std::byte> &bytes) const override final;

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
class BACKEND_EXPORT BRAMArrayBase : public ArrayBase
{
public:
    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Serialise backend-specific device object to uint32_t
    virtual void serialiseDeviceObject(std::vector<std::byte> &bytes) const override final;

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


//------------------------------------------------------------------------
// BRAMFieldArray
//------------------------------------------------------------------------
template<typename T>
class BRAMFieldArray : public IFieldArray, protected T
{
public:
    using T::T;

    //------------------------------------------------------------------------
    // IFieldArray virtuals
    //------------------------------------------------------------------------
    //! Sets field at offset to point to array
    virtual void setFieldArray(uint32_t fieldOffset, const ArrayBase *array) override final
    {
        // Serialise array's 'device object'
        std::vector<std::byte> bytes;
        array->serialiseDeviceObject(bytes);

        // Check serialised data is exactly 4 bytes
        assert(bytes.size() == 4);

        // Memcpy bytes into field offset
        std::memcpy(this->getHostPointer() + fieldOffset, 
                    bytes.data(), 4);
            
    }

    //! Copy field data to device
    virtual void pushFieldsToDevice() override final
    {
        this->pushToDevice();
    }
};

//----------------------------------------------------------------------------
// BackendFeNN
//----------------------------------------------------------------------------
//! Base class for FeNN backends
//! **NOTE** this is a temporary, simplified version of GeNN's BackendBase
class BACKEND_EXPORT BackendFeNN
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
    virtual std::unique_ptr<IFieldArray> createFieldArray(const Model &model, StateBase *state) const = 0;
    virtual std::unique_ptr<StateBase> createState() const = 0;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    std::vector<uint32_t> generateSimulationKernel(const std::vector<std::shared_ptr<const ProcessGroup>> &timestepProcessGroups, 
                                                   const std::vector <std::shared_ptr<const ProcessGroup>> &endProcessGroups,
                                                   uint32_t numTimesteps, const Model &model) const;

    std::vector<uint32_t> generateKernel(const std::vector <std::shared_ptr<const ProcessGroup>> &processGroups, const Model &model) const;

    std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const Variable> variable, 
                                           const Model::StateProcesses::mapped_type &processes,
                                           StateBase *state) const;

    std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const EventContainer> eventContainer, 
                                           const Model::StateProcesses::mapped_type &processes,
                                           StateBase *state) const;

protected:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Should kernels be generated with simulation or hardware signalling
    virtual bool shouldGenerateSimulationKernels() const = 0;

};
