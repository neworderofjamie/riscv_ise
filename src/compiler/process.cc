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
// Process
//----------------------------------------------------------------------------
Process::Process(const std::string &code)
{
    m_Tokens = GeNN::Utils::scanCode(code, "Process");
}

//----------------------------------------------------------------------------
// NeuronUpdateProcess
//----------------------------------------------------------------------------
NeuronUpdateProcess::NeuronUpdateProcess(const std::string &code, const ParameterMap &parameters, 
                                         const VariableMap &variables, const EventContainerMap &outputEvents)
:   Process(code), m_Parameters(parameters), m_Variables(variables), m_OutputEvents(outputEvents)
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
    
    // Batched = any output events batched
    // 
}

//----------------------------------------------------------------------------
// EventPropagationProcess
//----------------------------------------------------------------------------
EventPropagationProcess::EventPropagationProcess(const std::string &code, const EventContainer *inputEvents,
                                                 const ParameterMap &parameters, const VariableMap &synapseVariables,
                                                 const VariableMap &sourceVariables, const VariableMap &targetVariables)
:   Process(code), m_InputEvents(inputEvents), m_Parameters(parameters), m_SynapseVariables(synapseVariables), 
    m_SourceVariables(sourceVariables), m_TargetVariables(targetVariables)
{
    if(m_InputEvents == nullptr) {
        throw std::runtime_error("Event propagation process requires input events");
    }

    if(m_SynapseVariables.empty() && m_TargetVariables.empty()) {
        throw std::runtime_error("Event propagation process requires at least one synapse or target variable");
    }

    // Get number of source neurons from input events
    m_NumSourceNeurons = m_InputEvents->getShape().getNumNeurons();

    // Get number of target neurons from arbitrary synapsed or target variable
    m_NumTargetNeurons = (m_SynapseVariables.empty() ? m_SourceVariables.cbegin()->second->getShape().getNumNeurons() 
                          : m_SynapseVariables.cbegin()->second->getShape().getNumTargetNeurons());

    // Loop through synapse variables
    for(const auto &v : m_SynapseVariables) {
        // Check number of source neurons matches
        if(v.second->getShape().getNumSourceNeurons() != m_NumSourceNeurons) {
            throw std::runtime_error("Variable '" + v.first + "' with shape: " + v.second->getShape().toString() 
                                     + " is not compatible with event propagation process with " 
                                     + std::to_string(m_NumSourceNeurons) + " source neurons");
        }

         // Check number of target neurons matches
        if(v.second->getShape().getNumTargetNeurons() != m_NumTargetNeurons) {
            throw std::runtime_error("Variable '" + v.first + "' with shape: " + v.second->getShape().toString() 
                                     + " is not compatible with event propagation process with " 
                                     + std::to_string(m_NumTargetNeurons) + " target neurons");
        }
    }

    // Check all source variables have same number of neurons
    for(const auto &v : m_SourceVariables) {
        if(v.second->getShape().getNumNeurons() != m_NumSourceNeurons) {
            throw std::runtime_error("Variable '" + v.first + "' with shape: " + v.second->getShape().toString() 
                                     + " is not compatible with neuron update process with " 
                                     + std::to_string(m_NumSourceNeurons) + " source neurons");
        }
    }

     // Check all target variables have same number of neurons
    for(const auto &v : m_TargetVariables) {
        if(v.second->getShape().getNumNeurons() != m_NumTargetNeurons) {
            throw std::runtime_error("Variable '" + v.first + "' with shape: " + v.second->getShape().toString() 
                                     + " is not compatible with neuron update process with " 
                                     + std::to_string(m_NumTargetNeurons) + " target neurons");
        }
    }
}