#pragma once

// Standard C++ includes
#include <optional>

// Backend includes
#include "backend/runtime.h"

// FeNN compiler includes
#include "fenn/compiler/compiler.h"

// FeNN backend includes
#include "fenn/backend/backend_export.h"
#include "fenn/backend/memory_allocator.h"

//----------------------------------------------------------------------------
// FeNN::Backend::URAMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in FeNN's URAM
namespace FeNN::Backend
{
class FENN_BACKEND_EXPORT URAMArrayBase : public ::Backend::ArrayBase
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
    URAMArrayBase(const GeNN::Type::ResolvedType &type, size_t count, size_t core)
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
// FeNN::Backend::BRAMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in FeNN's BRAM
class FENN_BACKEND_EXPORT BRAMArrayBase  : public ::Backend::ArrayBase
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
class FENN_BACKEND_EXPORT LLMArrayBase : public ::Backend::ArrayBase
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
    LLMArrayBase(const GeNN::Type::ResolvedType &type, size_t count)
    :   ArrayBase(type, count)
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
class FENN_BACKEND_EXPORT DRAMArrayBase : public ::Backend::ArrayBase
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
class BACKEND_EXPORT URAMLLMArrayBase : public ::Backend::ArrayBase
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

    size_t getLLMCount() const{ return m_LLMCount; };
    size_t getLLMSizeBytes() const{ return m_LLMCount * getType().getValue().size; };

protected:
    URAMLLMArrayBase(const GeNN::Type::ResolvedType &type, size_t uramCount, size_t llmCount)
    :   ArrayBase(type, uramCount), m_LLMCount(llmCount)
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
    size_t m_LLMCount;
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
// FeNN::Backend::CoreStateBase
//----------------------------------------------------------------------------
class CoreStateBase
{
public:
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getBRAMAllocator() const{ return m_BRAMAllocator; }
    auto &getBRAMAllocator(){ return m_BRAMAllocator; }

    const auto &getURAMAllocator() const{ return m_URAMAllocator; }
    auto &getURAMAllocator(){ return m_URAMAllocator; }

    const auto &getLLMAllocator() const{ return m_LLMAllocator; }
    auto &getLLMAllocator(){ return m_LLMAllocator; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! Allocators for BRAM, URAM and Lane-Local Memories
    BRAMAllocator m_BRAMAllocator;
    URAMAllocator m_URAMAllocator;
    LLMAllocator m_LLMAllocator;
};

//----------------------------------------------------------------------------
// FeNN::Backend::Runtime
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT Runtime : public ::Backend::Runtime
{
    using KernelPtr = std::shared_ptr<const ::Model::Kernel>;
public:
   Runtime(const ::Model::Model &model, size_t numCores, bool useDRAMForWeights = false, bool keepParamsInRegisters = true, 
           Compiler::RoundingMode neuronUpdateRoundingMode = Compiler::RoundingMode::NEAREST);

protected:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<::Backend::ArrayBase> createURAMArray(const GeNN::Type::ResolvedType &type, size_t count, size_t core) const = 0;
    virtual std::unique_ptr<::Backend::ArrayBase> createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count, size_t core) const = 0;
    virtual std::unique_ptr<::Backend::ArrayBase> createLLMArray(const GeNN::Type::ResolvedType &type, size_t count, size_t core) const = 0;
    virtual std::unique_ptr<::Backend::ArrayBase> createDRAMArray(const GeNN::Type::ResolvedType &type, size_t count, size_t core) const = 0;
    virtual std::unique_ptr<::Backend::ArrayBase> createURAMLLMArray(const GeNN::Type::ResolvedType &type,
                                                                     size_t uramCount, size_t llmCount, size_t core) const = 0;
    
    //------------------------------------------------------------------------
    // Runtime virtuals
    //------------------------------------------------------------------------
    //! Create suitable array for event container
    virtual std::unique_ptr<::Backend::ArrayBase> createArray(std::shared_ptr<const ::Model::EventContainer> eventContainer) const override final;

    //! Create suitable array for performance counter
    virtual std::unique_ptr<::Backend::ArrayBase> createArray(std::shared_ptr<const ::Model::PerformanceCounter> performanceCounter) const override final;

    //! Create suitable array for variable
    virtual std::unique_ptr<::Backend::ArrayBase> createArray(std::shared_ptr<const ::Model::Variable> variable) const override final;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! Currently loaded kernel
    KernelPtr m_CurrentLoadedKernel;

    // Map of kernel pointers to code
    std::unordered_map<KernelPtr, std::vector<uint32_t>> m_KernelCode;

    size_t m_NumCores;
    bool m_UseDRAMForWeights;
    bool m_KeepParamsInRegisters;
    Compiler::RoundingMode m_NeuronUpdateRoundingMode;
};
}