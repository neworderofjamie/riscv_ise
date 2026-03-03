#pragma once

// Standard C++ includes
#include <optional>

// Frontend includes
#include "frontend/runtime.h"

// FeNN compiler includes
#include "fenn/compiler/compiler.h"

// FeNN backend includes
#include "fenn/backend/backend_export.h"
#include "fenn/backend/memory_allocator.h"
#include "fenn/backend/model.h"

// Forward declarations
namespace FeNN::Backend
{
class Runtime;
}

//----------------------------------------------------------------------------
// FeNN::Backend::URAMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in FeNN's URAM
namespace FeNN::Backend
{
class FENN_BACKEND_EXPORT URAMArrayBase : public Frontend::ArrayBase
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
    URAMArrayBase(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape)
    :   ArrayBase(type, shape)
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
// FeNN::Backend::BRAMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in FeNN's BRAM
class FENN_BACKEND_EXPORT BRAMArrayBase  : public Frontend::ArrayBase
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
    void setBRAMPointer(std::optional<uint32_t> bramPointer){ m_BRAMPointer = bramPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::optional<uint32_t> m_BRAMPointer;
};


//----------------------------------------------------------------------------
// FeNN::Backend::LLMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in FeNN's lane-local memories
class FENN_BACKEND_EXPORT LLMArrayBase : public Frontend::ArrayBase
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
    uint32_t getLLMPointer() const{ return m_LLMPointer.value(); }

protected:
    LLMArrayBase(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape)
    :   ArrayBase(type, shape)
    {
        if(type.getSize(0) != 2) {
            throw std::runtime_error("Only 16-bit types can be stored in LLM arrays");
        }
    }

    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    void setLLMPointer(std::optional<uint32_t> llmPointer){ m_LLMPointer = llmPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::optional<uint32_t> m_LLMPointer;
};

//----------------------------------------------------------------------------
// FeNN::Backend::DRAMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in DRAM, accessed using DMA controller
class FENN_BACKEND_EXPORT DRAMArrayBase : public Frontend::ArrayBase
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
    uint32_t getDRAMPointer() const{ return m_DRAMPointer.value(); }

protected:
    using ArrayBase::ArrayBase;

    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    void setDRAMPointer(std::optional<uint32_t> dramPointer){ m_DRAMPointer = dramPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::optional<uint32_t> m_DRAMPointer;
};

//----------------------------------------------------------------------------
// FeNN::Backend::URAMLLMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays which are allocated in URAM but also have a delayed input in LLM
//! Typically used for implementing neuron variables with dendritically-delayed input
class FENN_BACKEND_EXPORT URAMLLMArrayBase : public Frontend::ArrayBase
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
    uint32_t getLLMPointer() const{ return m_LLMPointer.value(); }

    const auto &getLLMShape() const{ return m_LLMShape; }
    size_t getLLMCount() const{ return m_LLMShape.getFlattenedSize(); };
    size_t getLLMSizeBytes() const{ return getLLMCount() * getType().getValue().size; };

protected:
    URAMLLMArrayBase(const GeNN::Type::ResolvedType &type, const Frontend::Shape &uramShape, 
                     const Frontend::Shape &llmShape)
    :   ArrayBase(type, uramShape), m_LLMShape(llmShape)
    {
        if(type.getSize(0) != 2) {
            throw std::runtime_error("Only 16-bit types can be stored in URAM/LLM arrays");
        }
    }

    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    void setURAMPointer(std::optional<uint32_t> uramPointer){ m_URAMPointer = uramPointer; }
    void setLLMPointer(std::optional<uint32_t> llmPointer){ m_LLMPointer = llmPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::optional<uint32_t> m_URAMPointer;
    std::optional<uint32_t> m_LLMPointer;
    Frontend::Shape m_LLMShape;
};


//------------------------------------------------------------------------
// FeNN::Backend::BRAMFieldArray
//------------------------------------------------------------------------
/*template<typename T>
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

        // Memcpy bytes into field offset
        std::memcpy(this->getHostPointer() + fieldOffset, 
                    bytes.data(), bytes.size());
    }

    //! Copy field data to device
    virtual void pushFieldsToDevice() override final
    {
        this->pushToDevice();
    }
};*/

//----------------------------------------------------------------------------
// FeNN::Backend::DeviceFeNN
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT DeviceFeNN : public Frontend::DeviceBase
{
public:
    DeviceFeNN(size_t deviceIndex, const Runtime &runtime);

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createURAMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape) = 0;
    virtual std::unique_ptr<Frontend::ArrayBase> createBRAMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape) = 0;
    virtual std::unique_ptr<Frontend::ArrayBase> createLLMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape) = 0;
    virtual std::unique_ptr<Frontend::ArrayBase> createDRAMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape) = 0;
    virtual std::unique_ptr<Frontend::ArrayBase> createURAMLLMArray(const GeNN::Type::ResolvedType &type,
                                                                     const Frontend::Shape &uramShape, 
                                                                     const Frontend::Shape &llmShape) = 0;
    //------------------------------------------------------------------------
    // DeviceBase virtuals
    //------------------------------------------------------------------------
    //! Create suitable array for event container on this device
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(std::shared_ptr<const Frontend::EventContainer> eventContainer,
                                                             const Frontend::Shape &shape) override final;

    //! Create suitable array for variable on this device
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(std::shared_ptr<const Frontend::Variable> variable,
                                                             const Frontend::Shape &shape) override final;

    //! Create suitable array for performance counter on this device
    virtual std::unique_ptr<Frontend::ArrayBase> createPerformanceCounter() override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getBRAMAllocator() const{ return m_BRAMAllocator; }
    auto &getBRAMAllocator(){ return m_BRAMAllocator; }

    const auto &getURAMAllocator() const{ return m_URAMAllocator; }
    auto &getURAMAllocator(){ return m_URAMAllocator; }

    const auto &getLLMAllocator() const{ return m_LLMAllocator; }
    auto &getLLMAllocator(){ return m_LLMAllocator; }

protected:
    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    const auto &getRuntime() const{ return m_Runtime.get(); }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! Allocators for BRAM, URAM and Lane-Local Memories
    BRAMAllocator m_BRAMAllocator;
    URAMAllocator m_URAMAllocator;
    LLMAllocator m_LLMAllocator;

    std::reference_wrapper<const Runtime> m_Runtime;
};

//----------------------------------------------------------------------------
// FeNN::Backend::Runtime
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT Runtime : public Frontend::Runtime
{
public:
   Runtime(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels, 
           size_t numDevices, bool useDRAMForWeights = false, bool keepParamsInRegisters = true, 
           Compiler::RoundingMode neuronUpdateRoundingMode = Compiler::RoundingMode::NEAREST,
           size_t dmaBufferSize = 512 * 1024);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getKernelCode(std::shared_ptr<const Frontend::Kernel> kernel) const{ return m_KernelCode.at(kernel); }
    bool shouldUseDRAMForWeights() const{ return m_UseDRAMForWeights; }
    size_t getDMABufferSize() const{ return m_DMABufferSize; }

    const auto &getModel() const{ return m_Model; }
    
private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    // Map of kernel pointers to code
    std::unordered_map<std::shared_ptr<const Frontend::Kernel>, 
                       std::vector<uint32_t>> m_KernelCode;

    bool m_UseDRAMForWeights;
    bool m_KeepParamsInRegisters;
    Compiler::RoundingMode m_NeuronUpdateRoundingMode;
    size_t m_DMABufferSize;

    Model m_Model;
};
}
