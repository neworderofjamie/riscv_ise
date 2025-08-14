#pragma once

// Standard C++ includes
#include <memory>
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cstdint>
#include <cstring>

// GeNN includes
#include "type.h"

// Compiler includes
#include "compiler/compiler.h"

// Backend include
#include "backend/backend_export.h"
#include "backend/memory_allocator.h"
#include "backend/memory_allocator.h"
#include "backend/model.h"
#include "backend/runtime.h"

// Forward declarations
class ProcessGroup;

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
    void setBRAMPointer(std::optional<uint32_t> bramPointer){ m_BRAMPointer = bramPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::optional<uint32_t> m_BRAMPointer;
};


//----------------------------------------------------------------------------
// LLMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in FeNN's lane-local memories
class BACKEND_EXPORT LLMArrayBase : public ArrayBase
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
// DRAMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in DRAM, accessed using DMA controller
class BACKEND_EXPORT DRAMArrayBase : public ArrayBase
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
// StateBase
//----------------------------------------------------------------------------
//! Base class for runtime state objects created by backend
class StateBase
{
public:
    StateBase() = default;
    virtual ~StateBase() = default;
    StateBase(const StateBase &) = delete;

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual void setInstructions(const std::vector<uint32_t> &instructions) = 0;
    virtual void run() = 0;

    virtual std::unique_ptr<ArrayBase> createURAMArray(const GeNN::Type::ResolvedType &type, size_t count) = 0;
    virtual std::unique_ptr<ArrayBase> createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count) = 0;
    virtual std::unique_ptr<ArrayBase> createLLMArray(const GeNN::Type::ResolvedType &type, size_t count) = 0;
    virtual std::unique_ptr<ArrayBase> createDRAMArray(const GeNN::Type::ResolvedType &type, size_t count) = 0;
    virtual std::unique_ptr<IFieldArray> createFieldArray(const Model &model) = 0;

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
// BackendFeNN
//----------------------------------------------------------------------------
//! Base class for FeNN backends
//! **NOTE** this is a temporary, simplified version of GeNN's BackendBase
class BACKEND_EXPORT BackendFeNN
{
public:
    BackendFeNN(bool useDRAMForWeights = false, bool keepParamsInRegisters = true,
                RoundingMode neuronUpdateRoundingMode = RoundingMode::NEAREST)
    :   m_UseDRAMForWeights(useDRAMForWeights), m_KeepParamsInRegisters(keepParamsInRegisters),
        m_NeuronUpdateRoundingMode(neuronUpdateRoundingMode)
    {}

    virtual ~BackendFeNN()
    {}

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<StateBase> createState() const = 0;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    std::vector<uint32_t> generateSimulationKernel(const std::vector<std::shared_ptr<const ProcessGroup>> &timestepProcessGroups, 
                                                   const std::vector <std::shared_ptr<const ProcessGroup>> &beginProcessGroups,
                                                   const std::vector <std::shared_ptr<const ProcessGroup>> &endProcessGroups,
                                                   uint32_t numTimesteps, const Model &model) const;

    std::vector<uint32_t> generateKernel(const std::vector <std::shared_ptr<const ProcessGroup>> &processGroups, const Model &model) const;

    std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const Variable> variable, 
                                           const Model::StateProcesses::mapped_type &processes,
                                           StateBase *state) const;

    std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const EventContainer> eventContainer, 
                                           const Model::StateProcesses::mapped_type &processes,
                                           StateBase *state) const;

    std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const PerformanceCounter> performanceCounter, 
                                           const Model::StateProcesses::mapped_type &processes,
                                           StateBase *state) const;

    std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const State> state, 
                                           int stateID, StateBase *backendState) const;

    //! Get mapping of IDs to state objects for any backend-specific state this model is going to require
    std::unordered_map<int, std::shared_ptr<State>> getRequiredStateObjects(const Model &model) const;

protected:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Should kernels be generated with simulation or hardware signalling
    virtual bool shouldGenerateSimulationKernels() const = 0;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    bool m_UseDRAMForWeights;
    bool m_KeepParamsInRegisters;
    RoundingMode m_NeuronUpdateRoundingMode;

};
