#pragma once

// Standard C++ includes
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cstdint>
#include <cstring>

// GeNN includes
#include "type.h"

// Model includes
#include "model/model.h"

// Compiler includes
#include "fenn/compiler/compiler.h"

// Backend include
#include "fenn/backend/backend_export.h"
#include "fenn/backend/memory_allocator.h"
#include "fenn/backend/runtime.h"

// Forward declarations
namespace Model
{
class EventContainer;
class PerformanceCounter;
class ProcessGroup;
class State;
class Variable;
}

//----------------------------------------------------------------------------
// FeNN::Backend::URAMArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays located in FeNN's URAM
namespace FeNN::Backend
{


//----------------------------------------------------------------------------
// FeNN::Backend::StateBase
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
    virtual void startRun() = 0;
    virtual void waitRun() = 0;

    virtual std::unique_ptr<ArrayBase> createURAMArray(const GeNN::Type::ResolvedType &type, size_t count) = 0;
    virtual std::unique_ptr<ArrayBase> createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count) = 0;
    virtual std::unique_ptr<ArrayBase> createLLMArray(const GeNN::Type::ResolvedType &type, size_t count) = 0;
    virtual std::unique_ptr<ArrayBase> createDRAMArray(const GeNN::Type::ResolvedType &type, size_t count) = 0;
    virtual std::unique_ptr<ArrayBase> createURAMLLMArray(const GeNN::Type::ResolvedType &type,
                                                          size_t uramCount, size_t llmCount) = 0;
    virtual std::unique_ptr<IFieldArray> createFieldArray(const ::Model::Model &model) = 0;

    virtual std::optional<unsigned int> getSOCPower() const = 0;

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
// FeNN::Backend::BackendFeNN
//----------------------------------------------------------------------------
//! Base class for FeNN backends
//! **NOTE** this is a temporary, simplified version of GeNN's BackendBase
class BACKEND_EXPORT BackendFeNN
{
public:
    BackendFeNN(bool useDRAMForWeights = false, bool keepParamsInRegisters = true,
                Compiler::RoundingMode neuronUpdateRoundingMode = Compiler::RoundingMode::NEAREST)
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
    std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const ::Model::Variable> variable, 
                                           const ::Model::Model::StateProcesses::mapped_type &processes,
                                           StateBase *state) const;

    std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const ::Model::EventContainer> eventContainer, 
                                           const ::Model::Model::StateProcesses::mapped_type &processes,
                                           StateBase *state) const;

    std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const ::Model::PerformanceCounter> performanceCounter, 
                                           const ::Model::Model::StateProcesses::mapped_type &processes,
                                           StateBase *state) const;

    std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const ::Model::State> state, 
                                           int stateID, StateBase *backendState) const;

    //! Get mapping of IDs to state objects for any backend-specific state this model is going to require
    std::unordered_map<int, std::shared_ptr<::Model::State>> getRequiredStateObjects(const ::Model::Model &model) const;

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
    Compiler::RoundingMode m_NeuronUpdateRoundingMode;

};
}