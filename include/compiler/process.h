#pragma once

// Standard C++ includes
#include <string>
#include <unordered_map>

// GeNN includes
#include "transpiler/token.h"

// Compiler includes
#include "model_component.h"

// Forward declarations
class EventContainer;
class Parameter;
class Variable;

using EventContainerMap = std::unordered_map<std::string, const EventContainer*>;
using ParameterMap = std::unordered_map<std::string, const Parameter*>;
using VariableMap = std::unordered_map<std::string, const Variable*>;

//----------------------------------------------------------------------------
// Process
//----------------------------------------------------------------------------
class Process : public ModelComponent
{

};

//----------------------------------------------------------------------------
// SpikeInputProcess
//----------------------------------------------------------------------------
class SpikeInputProcess : public AcceptableModelComponent<SpikeInputProcess, Process>
{
public:
    SpikeInputProcess(const EventContainer *outputEvents);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto *getOutputEvents() const{ return m_OutputEvents; }

    size_t getNumNeurons() const{ return m_NumNeurons; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    const EventContainer *m_OutputEvents;
    size_t m_NumNeurons;
};

//----------------------------------------------------------------------------
// NeuronUpdateProcess
//----------------------------------------------------------------------------
class NeuronUpdateProcess : public AcceptableModelComponent<NeuronUpdateProcess, Process>
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
// EventPropagationProcess
//----------------------------------------------------------------------------
class EventPropagationProcess : public AcceptableModelComponent<EventPropagationProcess, Process>
{
public:
    EventPropagationProcess(const EventContainer *inputEvents, const Variable *weight,
                            const Variable *target);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto *getInputEvents() const{ return m_InputEvents; }
    const auto *getWeight() const{ return m_Weight; }
    const auto *getTarget() const{ return m_Target; }

    size_t getNumSourceNeurons() const{ return m_NumSourceNeurons; }
    size_t getNumTargetNeurons() const{ return m_NumTargetNeurons; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    const EventContainer *m_InputEvents;
    const Variable *m_Weight;
    const Variable *m_Target;
    
    size_t m_NumSourceNeurons;
    size_t m_NumTargetNeurons;
};