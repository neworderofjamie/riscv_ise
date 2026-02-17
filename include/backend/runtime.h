#pragma once

// Standard C++ includes
#include <memory>
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cstdint>

// GeNN includes
#include "type.h"

// Backend includes
#include "backend/backend_export.h"

// Forward declarations
namespace Model
{
class Kernel;
class Model;
class State;
}

//----------------------------------------------------------------------------
// FeNN::Backend::ArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays created by backend
//! **NOTE** this is a temporary, simplified version of GeNN's ArrayBase
namespace Backend
{
class BACKEND_EXPORT ArrayBase
{
public:
    virtual ~ArrayBase()
    {
        m_Count = 0;
    }

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
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
/*class IFieldArray
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
};*/


//----------------------------------------------------------------------------
// Runtime
//----------------------------------------------------------------------------
class BACKEND_EXPORT Runtime
{
public:
    Runtime(const ::Model::Model &model);
   
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Run kernel
    virtual void run(std::shared_ptr<const ::Model::Kernel> kernel) = 0;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Allocate memory for model
    void allocate();
   
    // Get array associated with model state
    ArrayBase *getArray(std::shared_ptr<const ::Model::State> variable) const;


    //std::optional<unsigned int> getSOCPower() const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unordered_map<std::shared_ptr<const ::Model::State>, std::unique_ptr<ArrayBase>> m_Arrays;
    std::reference_wrapper<const ::Model::Model> m_Model;
    //std::unique_ptr<IFieldArray> m_FieldArray;
};
}