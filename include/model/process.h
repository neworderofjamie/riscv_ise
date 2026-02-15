#pragma once

// Standard C++ includes
#include <map>
#include <memory>
#include <string>
#include <variant>

// GeNN includes
#include "transpiler/token.h"

// Compiler includes
#include "model/model_export.h"
#include "model/model_component.h"

// Forward declarations
namespace Model
{
class EventContainer;
class Parameter;
class Variable;
}

namespace Model
{
using VariablePtr = std::shared_ptr<const Variable>;
using VariablePtrBackendState = std::variant<VariablePtr, int>;
using EventContainerMap = std::map<std::string, std::shared_ptr<EventContainer>>;
using ParameterMap = std::map<std::string, std::shared_ptr<const Parameter>>;
using VariableMap = std::map<std::string, VariablePtr>;


//----------------------------------------------------------------------------
// Model::NeuronUpdateProcess
//----------------------------------------------------------------------------
class MODEL_EXPORT NeuronUpdateProcess : public AcceptableModelComponent<NeuronUpdateProcess, Process>
{
public:
    NeuronUpdateProcess(Private, const std::string &code, const ParameterMap &parameters, 
                        const VariableMap &variables, const EventContainerMap &outputEvents,
                        const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getParameters() const{ return m_Parameters; }
    const auto &getVariables() const{ return m_Variables; }
    const auto &getOutputEvents() const{ return m_OutputEvents; }

    const auto &getTokens() const{ return m_Tokens; }

    size_t getNumNeurons() const{ return m_NumNeurons; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    ParameterMap m_Parameters;
    VariableMap m_Variables;
    EventContainerMap m_OutputEvents;

    std::vector<GeNN::Transpiler::Token> m_Tokens;

    size_t m_NumNeurons;
};

//----------------------------------------------------------------------------
// Model::EventPropagationProcess
//----------------------------------------------------------------------------
class MODEL_EXPORT EventPropagationProcess : public AcceptableModelComponent<EventPropagationProcess, Process>
{
public:
    EventPropagationProcess(Private, std::shared_ptr<const EventContainer> inputEvents, 
                            VariablePtr weight, VariablePtr target,
                            size_t numSparseConnectivityBits, size_t numDelayBits,
                            const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getInputEvents() const{ return m_InputEvents; }
    const auto getWeight() const{ return m_Weight; }
    const auto getTarget() const{ return m_Target; }

    size_t getNumSourceNeurons() const{ return m_NumSourceNeurons; }
    size_t getNumTargetNeurons() const{ return m_NumTargetNeurons; }
    size_t getMaxRowLength() const{ return m_MaxRowLength; }

    size_t getNumSparseConnectivityBits() const{ return m_NumSparseConnectivityBits; }
    size_t getNumDelayBits() const{ return m_NumDelayBits; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::shared_ptr<const EventContainer> m_InputEvents;
    VariablePtr m_Weight;
    VariablePtr m_Target;
    
    size_t m_NumSourceNeurons;
    size_t m_NumTargetNeurons;
    size_t m_MaxRowLength;

    size_t m_NumSparseConnectivityBits;
    size_t m_NumDelayBits;
};

//----------------------------------------------------------------------------
// Model::RNGInitProcess
//----------------------------------------------------------------------------
class MODEL_EXPORT RNGInitProcess : public AcceptableModelComponent<RNGInitProcess, Process>
{
public:
    RNGInitProcess(Private, VariablePtr seed, const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getSeed() const{ return m_Seed; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::shared_ptr<const Variable> m_Seed;
};

//----------------------------------------------------------------------------
// Model::MemsetProcess
//----------------------------------------------------------------------------
//! Process for memsetting variables
class MODEL_EXPORT MemsetProcess : public AcceptableModelComponent<MemsetProcess, Process>
{
public:
    MemsetProcess(Private, VariablePtrBackendState target, const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getTarget() const{ return m_Target; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    VariablePtrBackendState m_Target;
};

//----------------------------------------------------------------------------
// Model::BroadcastProcess
//----------------------------------------------------------------------------
//! Process for 'broadcasting' one scalar array across multiple vector lanes
class MODEL_EXPORT BroadcastProcess : public AcceptableModelComponent<BroadcastProcess, Process>
{
public:
    BroadcastProcess(Private, VariablePtr source, VariablePtrBackendState target, const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getSource() const{ return m_Source; }
    const auto getTarget() const{ return m_Target; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    VariablePtr m_Source;
    VariablePtrBackendState m_Target;
};
}