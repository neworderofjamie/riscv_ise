// Standard C++ includes
#include <stdexcept>

// GeNN includes
#include "gennUtils.h"

// Compiler includes
#include "compiler/event_container.h"
#include "compiler/parameter.h"
#include "compiler/process.h"
#include "compiler/variable.h"

//----------------------------------------------------------------------------
// NeuronUpdateProcess
//----------------------------------------------------------------------------
NeuronUpdateProcess::NeuronUpdateProcess(Private, const std::string &code, const ParameterMap &parameters, 
                                         const VariableMap &variables, const EventContainerMap &outputEvents,
                                         const std::string &name)
:   AcceptableModelComponent<NeuronUpdateProcess, Process>(name), m_Parameters(parameters), 
    m_Variables(variables), m_OutputEvents(outputEvents)
{
    if(m_Variables.empty() && m_OutputEvents.empty()) {
        throw std::runtime_error("Neuron update process requires at least one variable or output event");
    }

    // Get number of neurons from arbitrary variable or output event
    m_NumNeurons = (m_Variables.empty() ? m_OutputEvents.cbegin()->second->getShape().getNumNeurons() 
                    : m_Variables.cbegin()->second->getShape().getNumNeurons());

    // Check all variables have same number of neurons
    for(const auto &v : m_Variables) {
        if(v.second->getShape().getNumNeurons() != m_NumNeurons) {
            throw std::runtime_error("Variable '" + v.first + "' with shape: " + v.second->getShape().toString() 
                                     + " is not compatible with neuron update process with " + std::to_string(m_NumNeurons) + " neurons");
        }
    }

    // Check all output events have same number of neurons
    for(const auto &o : m_OutputEvents) {
        if(o.second->getShape().getNumNeurons() != m_NumNeurons) {
            throw std::runtime_error("Output events '" + o.first + "' with shape: " + o.second->getShape().toString()
                                     + " is not compatible with neuron update process with " + std::to_string(m_NumNeurons) + " neurons");
        }
    }
    
    // Parse code
    m_Tokens = GeNN::Utils::scanCode(code, "NeuronUpdateProcess");

    // Batched = any output events batched
    // 
}

//----------------------------------------------------------------------------
// EventPropagationProcess
//----------------------------------------------------------------------------
EventPropagationProcess::EventPropagationProcess(Private, std::shared_ptr<const EventContainer> inputEvents, 
                                                 std::shared_ptr<const Variable> weight, std::shared_ptr<const Variable> target,
                                                 const std::string &name)
:   AcceptableModelComponent<EventPropagationProcess, Process>(name), m_InputEvents(inputEvents), 
    m_Weight(weight), m_Target(target)
{
    if(m_InputEvents == nullptr) {
        throw std::runtime_error("Event propagation process requires input events");
    }

    if(m_Weight == nullptr) {
        throw std::runtime_error("Event propagation process requires weight variable");
    }

    if(m_Target == nullptr) {
        throw std::runtime_error("Event propagation process requires target variable");
    }

    // Get number of source neurons from input events
    m_NumSourceNeurons = m_InputEvents->getShape().getNumNeurons();

    // Get number of target neurons from target variable
    m_NumTargetNeurons = m_Target->getShape().getNumNeurons();

    // Check weight number of source neurons matches
    if(m_Weight->getShape().getNumSourceNeurons() != m_NumSourceNeurons) {
        throw std::runtime_error("Weight with shape: " + weight->getShape().toString() 
                                 + " is not compatible with event propagation process with " 
                                 + std::to_string(m_NumSourceNeurons) + " source neurons");
    }

    // Check weight number of target neurons matches
    if(m_Weight->getShape().getNumTargetNeurons() != m_NumTargetNeurons) {
        throw std::runtime_error("Weight with shape: " + weight->getShape().toString() 
                                 + " is not compatible with event propagation process with " 
                                 + std::to_string(m_NumTargetNeurons) + " target neurons");
    }
    
}