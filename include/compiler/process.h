#pragma once

// Standard C++ includes
#include <string>
#include <unordered_map>

// GeNN includes
#include "transpiler/token.h"

// Forward declarations
class EventContainer;
class Parameter;
class Variable;

//----------------------------------------------------------------------------
// Process
//----------------------------------------------------------------------------
class Process
{
public:
    using EventContainerMap = std::unordered_map<std::string, const EventContainer*>;
    using ParameterMap = std::unordered_map<std::string, const Parameter*>;
    using VariableMap = std::unordered_map<std::string, const Variable*>;

    Process(const Process&) = delete;
    virtual ~Process()
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getTokens() const{ return m_Tokens; }

protected:
    Process(const std::string &code);

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<GeNN::Transpiler::Token> m_Tokens;
};

//----------------------------------------------------------------------------
// NeuronUpdateProcess
//----------------------------------------------------------------------------
class NeuronUpdateProcess : public Process
{
public:
    NeuronUpdateProcess(const std::string &code, const ParameterMap &parameters, 
                        const VariableMap &variables, const EventContainerMap &outputEvents);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getParameters() const{ return m_Parameters; }
    const auto &getVariables() const{ return m_Variables; }
    const auto &getOutputEvents() const{ return m_OutputEvents; }

    size_t getNumNeurons() const{ return m_NumNeurons; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    ParameterMap m_Parameters;
    VariableMap m_Variables;
    EventContainerMap m_OutputEvents;

    size_t m_NumNeurons;
};

//----------------------------------------------------------------------------
// EventPropagationProcess
//----------------------------------------------------------------------------
class EventPropagationProcess : public Process
{
public:
    EventPropagationProcess(const std::string &code, const EventContainer *inputEvents,
                            const ParameterMap &parameters, const VariableMap &synapseVariables,
                            const VariableMap &sourceVariables, const VariableMap &targetVariables);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto *getInputEvents() const{ return m_InputEvents; }
    const auto &getParameters() const{ return m_Parameters; }
    const auto &getSynapseVariables() const{ return m_SynapseVariables; }
    const auto &getSourceVariables() const{ return m_SourceVariables; }
    const auto &getTargetVariables() const{ return m_TargetVariables; }

    size_t getNumSourceNeurons() const{ return m_NumSourceNeurons; }
    size_t getNumTargetNeurons() const{ return m_NumTargetNeurons; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    const EventContainer *m_InputEvents;
    ParameterMap m_Parameters;
    VariableMap m_SynapseVariables;
    VariableMap m_SourceVariables;
    VariableMap m_TargetVariables;

    size_t m_NumSourceNeurons;
    size_t m_NumTargetNeurons;
};