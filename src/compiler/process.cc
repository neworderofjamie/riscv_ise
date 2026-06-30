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

    // Check all output have same number of neurons
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
// EventPropagationProcessBase
//----------------------------------------------------------------------------
EventPropagationProcessBase::EventPropagationProcessBase(std::shared_ptr<const EventContainer> inputEvents, const std::string &name)
:   Process(name), m_InputEvents(inputEvents)
{
    if(m_InputEvents == nullptr) {
        throw std::runtime_error("Event propagation process requires input events");
    }

    // Get number of source neurons from input events
    m_NumSourceNeurons = m_InputEvents->getShape().getNumNeurons();
    
}

//----------------------------------------------------------------------------
// EventPropagationProcess
//----------------------------------------------------------------------------
EventPropagationProcess::EventPropagationProcess(Private, std::shared_ptr<const EventContainer> inputEvents, 
                                                 VariablePtr weight, VariablePtr target, size_t numSparseConnectivityBits, 
                                                 size_t numDelayBits, const std::string &name)
:   AcceptableModelComponent<EventPropagationProcess, EventPropagationProcessBase>(inputEvents, name),
    m_Weight(weight), m_Target(target), m_NumSparseConnectivityBits(numSparseConnectivityBits),
    m_NumDelayBits(numDelayBits)
{
    if(m_Weight == nullptr) {
        throw std::runtime_error("Event propagation process requires weight variable");
    }

    if(m_Target == nullptr) {
        throw std::runtime_error("Event propagation process requires target variable");
    }

    // Get number of target neurons from target variable
    m_NumTargetNeurons = m_Target->getShape().getNumNeurons();

    // Get maximum row length from weight variable shape
    m_MaxRowLength = m_Weight->getShape().getNumTargetNeurons();

    // Check weight number of source neurons matches
    if(m_Weight->getShape().getNumSourceNeurons() != getNumSourceNeurons()) {
        throw std::runtime_error("Weight with shape: " + weight->getShape().toString() 
                                 + " is not compatible with event propagation process with " 
                                 + std::to_string(getNumSourceNeurons()) + " source neurons");
    }

    // Check delays and sparsity are not being combined
    if(m_NumDelayBits > 0 && m_NumSparseConnectivityBits > 0) {
        throw std::runtime_error("Event propagation processes with both events "
                                 "and delays are not currently supported");
    }

    // Check weight number of target neurons matches if no sparsity
    if(m_NumSparseConnectivityBits == 0 && m_MaxRowLength != m_NumTargetNeurons) {
        throw std::runtime_error("Weight with shape: " + weight->getShape().toString() 
                                 + " is not compatible with dense event propagation process with " 
                                 + std::to_string(m_NumTargetNeurons) + " target neurons");
    }

    if (m_Weight->getNumBufferTimesteps() != 1) {
        throw std::runtime_error("Weight has more than 1 buffer timestep which isn't "
                                 "currently supported by event propagation processes");
    }

    // If there are no delays, check target only has one buffer timestep
    if (m_NumDelayBits == 0) {
        if(m_Target->getNumBufferTimesteps() != 1) {
            throw std::runtime_error("Target has more than 1 buffer timestep "
                                     "but no delay bits are specified");
        }
    }
    // Otherwise, check buffer size matches
    // **YUCK** the fact delays are actually addresses to 2 byte things is very FeNN-specific
    else {
        if(m_Target->getNumBufferTimesteps() != (1 << (m_NumDelayBits - 1))) {
            throw std::runtime_error("Number of target buffer timestep does not "
                                     "match specified number of delay bits");
        }
    }
    
}

//----------------------------------------------------------------------------
// STDPEventPropagationProcess
//----------------------------------------------------------------------------
STDPEventPropagationProcess::STDPEventPropagationProcess(Private, std::shared_ptr<const EventContainer> inputEvents, 
                                                         VariablePtr x, VariablePtr target, VariablePtr preTimeSinceLastSpike,
                                                         VariablePtr vPre, VariablePtr cPre,
                                                         float thetaX, float a, float b, 
                                                         float alpha, float beta, 
                                                         float jPlus, float jMinus, 
                                                         float thetaV,float thetaLowUp, 
                                                         float thetaLowDown, float thetaHighUp, 
                                                         float thetaHighDown,
                                                         const std::string &name)
:   AcceptableModelComponent<STDPEventPropagationProcess, EventPropagationProcessBase>(inputEvents, name),
    m_X(x), m_Target(target), m_PreTimeSinceLastSpike(preTimeSinceLastSpike), m_VPre(vPre), m_CPre(cPre),
    m_ThetaX(thetaX), m_A(a),
    m_B(b),m_Alpha(alpha),
    m_Beta(beta), m_JPlus(jPlus), m_JMinus(jMinus),
    m_ThetaV(thetaV), m_ThetaLowUp(thetaLowUp), m_ThetaLowDown(thetaLowDown),
    m_ThetaHighUp(thetaHighUp), m_ThetaHighDown(thetaHighDown)
{
    if(m_X == nullptr) {
        throw std::runtime_error("STDP Event propagation process requires X variable");
    }

    if(m_Target == nullptr) {
        throw std::runtime_error("STDP Event propagation process requires target variable");
    }


    if(m_PreTimeSinceLastSpike == nullptr) {
        throw std::runtime_error("STDP Event propagation process requires presynaptic time since last spike variable");
    }

    if(m_VPre == nullptr) {
        throw std::runtime_error("STDP Event propagation process requires postsynaptic voltage variable");
    }

    if(m_CPre == nullptr) {
        throw std::runtime_error("STDP Event propagation process requires postsynaptic calcium variable");
    }

    // Get number of target neurons from target variable
    m_NumTargetNeurons = m_Target->getShape().getNumNeurons();

    // Get maximum row length from weight variable shape
    m_MaxRowLength = m_X->getShape().getNumTargetNeurons();

    // Check weight number of source neurons matches
    if(m_X->getShape().getNumSourceNeurons() != getNumSourceNeurons()) {
        throw std::runtime_error("Weight with shape: " + m_X->getShape().toString()
                                 + " is not compatible with stdp event propagation process with " 
                                 + std::to_string(getNumSourceNeurons()) + " source neurons");
    }



    if (m_X->getNumBufferTimesteps() != 1) {
        throw std::runtime_error("Weight has more than 1 buffer timestep which isn't "
                                 "currently supported by stdp event propagation processes");
    }

    
    // Check presynaptic time since last spike shape is compatible
    if (getPreTimeSinceLastSpike()->getShape().getNumNeurons() != getNumSourceNeurons()) {
        throw std::runtime_error("Presynaptic time since last spike with shape: " 
                                 + getPreTimeSinceLastSpike()->getShape().toString() 
                                 + " is not compatible with STDP event propagation process with " 
                                 + std::to_string(getNumSourceNeurons()) + " source neurons");
    }

    // Check postsymaptic calcium shape is compatible
    if (m_CPre->getShape().getNumNeurons() != getNumTargetNeurons()) {
        throw std::runtime_error("Postsynaptic calcium with shape: " + m_CPre->getShape().toString() 
                                 + " is not compatible with STDP event propagation process with " 
                                 + std::to_string(getNumTargetNeurons()) + " target neurons");
    }
    // Check postsymaptic voltage shape is compatible
    if (m_VPre->getShape().getNumNeurons() != getNumTargetNeurons()) {
        throw std::runtime_error("Postsynaptic voltage with shape: " + m_VPre->getShape().toString() 
                                 + " is not compatible with STDP event propagation process with " 
                                 + std::to_string(getNumTargetNeurons()) + " target neurons");
    }

}


//----------------------------------------------------------------------------
// RNGInitProcess
//----------------------------------------------------------------------------
RNGInitProcess::RNGInitProcess(Private, VariablePtr seed, const std::string &name)
:   AcceptableModelComponent<RNGInitProcess, Process>(name), m_Seed(seed)
{
    if(m_Seed == nullptr) {
        throw std::runtime_error("RNG init process requires seed");
    }
}

//----------------------------------------------------------------------------
// MemsetProcess
//----------------------------------------------------------------------------
MemsetProcess::MemsetProcess(Private, VariablePtrBackendState target, const std::string &name)
:   AcceptableModelComponent<MemsetProcess, Process>(name), m_Target(target)
{
    // If target is a variable
    // **NOTE** if target is a backend-specific state object, there's nothing we can check yet
    if(std::holds_alternative<VariablePtr>(m_Target)) {
        auto targetVar = std::get<VariablePtr>(m_Target);
        if(targetVar == nullptr) {
            throw std::runtime_error("Memset process requires target");
        }
    }
}

//----------------------------------------------------------------------------
// BroadcastProcess
//----------------------------------------------------------------------------
BroadcastProcess::BroadcastProcess(Private, VariablePtr source, VariablePtrBackendState target, const std::string &name)
:   AcceptableModelComponent<BroadcastProcess, Process>(name), m_Source(source), m_Target(target)
{
    if(m_Source == nullptr) {
        throw std::runtime_error("Broadcast process requires source");
    }

    if (m_Source->getNumBufferTimesteps() != 1) {
        throw std::runtime_error("Broadcast process source has more than 1 buffer "
                                 "timestep which isn't currently supported");
    }

    if(m_Source->getShape().getDims().size() != 1) {
        throw std::runtime_error("Multi-dimensional sources aren't currently "
                                 "supported by broadcast processes");
    }

    // If target is a variable
    // **NOTE** if target is a backend-specific state object, there's nothing we can check yet
    if(std::holds_alternative<VariablePtr>(m_Target)) {
        auto targetVar = std::get<VariablePtr>(m_Target);
        if(targetVar == nullptr) {
            throw std::runtime_error("Broadcast process requires target");
        }

        if (targetVar->getNumBufferTimesteps() != 1) {
            throw std::runtime_error("Broadcast process target has more than 1 buffer "
                                     "timestep which isn't currently supported");
        }

        if(targetVar->getShape().getDims().size() != 2) {
            throw std::runtime_error("Broadcast process currently required 2 dimensional target");
        }

        if(targetVar->getShape().getDims().at(0) != m_Source->getShape().getDims().at(0)) {
            throw std::runtime_error("Broadcast process requires first dimension of source and target to match");
        }

        if (m_Source->getType() != targetVar->getType()) {
            throw std::runtime_error("Broadcast process requires source and target with same shape");
        }
    }
}
