#pragma once

// Standard C++ includes
#include <optional>

// Frontend includes
#include "frontend/runtime.h"

// FeNN compiler includes
#include "fenn/compiler/compiler.h"

// FeNN backend includes
#include "fenn/backend/backend_export.h"
#include "fenn/backend/fields.h"
#include "fenn/backend/memory_allocator.h"

// Forward declarations
namespace Frontend
{
class EventSource;
class ProcessGroup;
}
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
    auto getURAMPointer() const{ return m_URAMPointer; }

protected:
    URAMArrayBase(const CompilerFrontend::Type::ResolvedType &type, 
                  const std::vector<size_t> &shape, const std::vector<size_t> &strides)
    :   ArrayBase(type, shape, strides)
    {
        if(type.getSize() != 2) {
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
    auto getBRAMPointer() const{ return m_BRAMPointer; }

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
    auto getLLMPointer() const{ return m_LLMPointer; }

protected:
    LLMArrayBase(const CompilerFrontend::Type::ResolvedType &type, 
                 const std::vector<size_t> &shape, const std::vector<size_t> &strides)
    :   ArrayBase(type, shape, strides)
    {
        if(type.getSize() != 2) {
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
    auto getDRAMPointer() const{ return m_DRAMPointer; }

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
// FeNN::Backend::DeviceFeNN
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT DeviceFeNN : public Frontend::DeviceBase
{
public:
    DeviceFeNN(size_t deviceIndex, Runtime &runtime);

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<URAMArrayBase> createURAMArray(const CompilerFrontend::Type::ResolvedType &type,
                                                           const std::vector<size_t> &shape, const std::vector<size_t> &strides) = 0;
    virtual std::unique_ptr<BRAMArrayBase> createBRAMArray(const CompilerFrontend::Type::ResolvedType &type,
                                                           const std::vector<size_t> &shape, const std::vector<size_t> &strides) = 0;
    virtual std::unique_ptr<LLMArrayBase> createLLMArray(const CompilerFrontend::Type::ResolvedType &type,
                                                         const std::vector<size_t> &shape, const std::vector<size_t> &strides) = 0;
    virtual std::unique_ptr<DRAMArrayBase> createDRAMArray(const CompilerFrontend::Type::ResolvedType &type,
                                                           const std::vector<size_t> &shape, const std::vector<size_t> &strides) = 0;

    //------------------------------------------------------------------------
    // DeviceBase virtuals
    //------------------------------------------------------------------------
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

    void createFieldArray(uint32_t numFieldBytes);

    BRAMArrayBase *getFieldArray(){ return m_FieldArray.get(); }
    const BRAMArrayBase *getFieldArray() const { return m_FieldArray.get(); }

protected:
    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    const auto &getRuntime() const{ return m_Runtime.get(); }
    auto &getRuntime(){ return m_Runtime.get(); }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! Allocators for BRAM, URAM and Lane-Local Memories
    BRAMAllocator m_BRAMAllocator;
    URAMAllocator m_URAMAllocator;
    LLMAllocator m_LLMAllocator;

    std::unique_ptr<BRAMArrayBase> m_FieldArray;

    std::reference_wrapper<Runtime> m_Runtime;
};

//----------------------------------------------------------------------------
// FeNN::Backend::Runtime
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT Runtime : public Frontend::Runtime
{
public:
   Runtime(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels, 
           size_t numDevices, bool generateSimulationKernels, bool useDRAMForWeights = false, 
           bool keepParamsInRegisters = true, Compiler::RoundingMode neuronUpdateRoundingMode = Compiler::RoundingMode::NEAREST,
           size_t dmaBufferSize = 512 * 1024);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getKernelCode(std::shared_ptr<const Frontend::Kernel> kernel) const{ return m_KernelCode.at(kernel); }
    bool shouldUseDRAMForWeights() const{ return m_UseDRAMForWeights; }
    bool shouldKeepParamsInRegisters() const{ return m_KeepParamsInRegisters; }
    size_t getDMABufferSize() const{ return m_DMABufferSize; }
    auto getNeuronRoundingMode() const{ return m_NeuronUpdateRoundingMode; }
    
    //! Get merged event sources
    const auto &getMergedEventSources() const { return m_MergedEventSources; }

protected:
    //------------------------------------------------------------------------
    // Runtime virtuals
    //------------------------------------------------------------------------
    //! Backend-specific logic to run at beginning of allocate function
    virtual void allocatePreamble() override final;

    //! Backend-specific logic to run at end of allocate function
    virtual void allocatePostamble() override final;

private:
    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    void populateFields(size_t p, const std::pair<uint32_t, MergedFields> &mergedFields,
                        std::shared_ptr<const Frontend::ModelComponent> owner);

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    // Map of kernel pointers to code
    std::unordered_map<std::shared_ptr<const Frontend::Kernel>, 
                       std::vector<uint32_t>> m_KernelCode;

    //! Map from process groups to start addresses and merged fields associated with processes
    std::unordered_map<std::shared_ptr<const Frontend::ProcessGroup>,
                       std::vector<std::pair<uint32_t, MergedFields>>> m_MergedProcessFields;

    //! Map from process groups to start addresses and merged fields associated with event sources
    std::unordered_map<std::shared_ptr<const Frontend::ProcessGroup>,
                       std::vector<std::pair<uint32_t, MergedFields>>> m_MergedEventSourceFields;

    // Map of process groups to merged process groups
    std::unordered_map<std::shared_ptr<const Frontend::ProcessGroup>,
                       std::vector<Frontend::Merged<Frontend::EventSource>>> m_MergedEventSources;

    bool m_UseDRAMForWeights;
    bool m_KeepParamsInRegisters;
    Compiler::RoundingMode m_NeuronUpdateRoundingMode;
    size_t m_DMABufferSize;
    size_t m_NumFieldBytes;
};
}
