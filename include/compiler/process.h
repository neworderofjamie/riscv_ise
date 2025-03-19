#pragma once

// Standard C++ includes
#include <memory>
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

using EventContainerMap = std::unordered_map<std::string, std::shared_ptr<EventContainer>>;
using ParameterMap = std::unordered_map<std::string, std::shared_ptr<const Parameter>>;
using VariableMap = std::unordered_map<std::string, std::shared_ptr<const Variable>>;

//----------------------------------------------------------------------------
// NeuronUpdateProcess
//----------------------------------------------------------------------------
class NeuronUpdateProcess : public AcceptableModelComponent<NeuronUpdateProcess, Process>
{
public:
    NeuronUpdateProcess(Private, const std::string &code, const ParameterMap &parameters, 
                        const VariableMap &variables, const EventContainerMap &outputEvents,
                        const std::string &name);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getParameters() const{ return m_Parameters; }
    const auto &getVariables() const{ return m_Variables; }
    const auto &getOutputEvents() const{ return m_OutputEvents; }

    const auto &getTokens() const{ return m_Tokens; }

    size_t getNumNeurons() const{ return m_NumNeurons; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<NeuronUpdateProcess> create(const std::string &code, const ParameterMap &parameters, 
                                                       const VariableMap &variables, const EventContainerMap &outputEvents = {},
                                                       const std::string &name = "")
    {
        return std::make_shared<NeuronUpdateProcess>(Private(), code, parameters, variables, outputEvents, name);
    }

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
    EventPropagationProcess(Private, std::shared_ptr<const EventContainer> inputEvents, 
                            std::shared_ptr<const Variable> weight, std::shared_ptr<const Variable> target,
                            const std::string &name);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getInputEvents() const{ return m_InputEvents; }
    const auto getWeight() const{ return m_Weight; }
    const auto getTarget() const{ return m_Target; }

    size_t getNumSourceNeurons() const{ return m_NumSourceNeurons; }
    size_t getNumTargetNeurons() const{ return m_NumTargetNeurons; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventPropagationProcess> create(std::shared_ptr<const EventContainer> inputEvents, 
                                                           std::shared_ptr<const Variable> weight,
                                                           std::shared_ptr<const Variable> target, 
                                                           const std::string &name = "")
    {
        return std::make_shared<EventPropagationProcess>(Private(), inputEvents, weight, target, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::shared_ptr<const EventContainer> m_InputEvents;
    std::shared_ptr<const Variable> m_Weight;
    std::shared_ptr<const Variable> m_Target;
    
    size_t m_NumSourceNeurons;
    size_t m_NumTargetNeurons;
};