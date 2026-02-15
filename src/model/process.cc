// Standard C++ includes
#include <stdexcept>

// GeNN includes
#include "gennUtils.h"

// Compiler includes
#include "model/event_container.h"
#include "model/parameter.h"
#include "model/process.h"
#include "model/variable.h"

#define UPDATE_HASH_CLASS_NAME(CLASS_NAME) GeNN::Utils::updateHash(#CLASS_NAME, hash);

//----------------------------------------------------------------------------
// Model::NeuronUpdateProcess
//----------------------------------------------------------------------------
namespace Model
{
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
std::vector<std::shared_ptr<const State>> NeuronUpdateProcess::getState() const
{
    std::vector<std::shared_ptr<const State>> state;
    std::transform(getVariables().cbegin(), getVariables().cend(), std::back_inserter(state),
                   [](const auto &v){ return v.second; });
    std::transform(getOutputEvents().cbegin(), getOutputEvents().cend(), std::back_inserter(state),
                   [](const auto &o){ return o.second; });
    return state;
}
//----------------------------------------------------------------------------
boost::uuids::detail::sha1::digest_type NeuronUpdateProcess::getMergeHashDigest() const
{
    using namespace GeNN::Utils;
    boost::uuids::detail::sha1 hash;

    UPDATE_HASH_CLASS_NAME(NeuronUpdateProcess);

    // Parameters
    updateHash(getParameters().size(), hash);
    for(const auto &p : getParameters()) {
        updateHash(p.first, hash);
        p.second->updateMergeHash(hash);
    }

    // Variables
    updateHash(getVariables().size(), hash);
    for(const auto &v : getVariables()) {
        updateHash(v.first, hash);
        v.second->updateMergeHash(hash);
    }

    // Output events
    updateHash(getOutputEvents().size(), hash);
    for(const auto &e : getOutputEvents()) {
        updateHash(e.first, hash);
        e.second->updateMergeHash(hash);
    }
    return hash.get_digest();
}

//----------------------------------------------------------------------------
// EventPropagationProcess
//----------------------------------------------------------------------------
EventPropagationProcess::EventPropagationProcess(Private, std::shared_ptr<const EventContainer> inputEvents, 
                                                 VariablePtr weight, VariablePtr target, size_t numSparseConnectivityBits, 
                                                 size_t numDelayBits, const std::string &name)
:   AcceptableModelComponent<EventPropagationProcess, Process>(name), m_InputEvents(inputEvents), 
    m_Weight(weight), m_Target(target), m_NumSparseConnectivityBits(numSparseConnectivityBits),
    m_NumDelayBits(numDelayBits)
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

    // Get maximum row length from weight variable shape
    m_MaxRowLength = m_Weight->getShape().getNumTargetNeurons();

    // Check weight number of source neurons matches
    if(m_Weight->getShape().getNumSourceNeurons() != m_NumSourceNeurons) {
        throw std::runtime_error("Weight with shape: " + weight->getShape().toString() 
                                 + " is not compatible with event propagation process with " 
                                 + std::to_string(m_NumSourceNeurons) + " source neurons");
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
std::vector<std::shared_ptr<const State>> EventPropagationProcess::getState() const
{
    return {getInputEvents(), getWeight(), getTarget()};
}
//----------------------------------------------------------------------------
boost::uuids::detail::sha1::digest_type EventPropagationProcess::getMergeHashDigest() const
{
    using namespace GeNN::Utils;
    boost::uuids::detail::sha1 hash;

    UPDATE_HASH_CLASS_NAME(EventPropagationProcess);

    // Input events
    getInputEvents()->updateMergeHash(hash);

    // Weights and targets
    getWeight()->updateMergeHash(hash);
    getTarget()->updateMergeHash(hash);

    // Formats
    updateHash(getNumDelayBits(), hash);
    updateHash(getNumSparseConnectivityBits(), hash);

    return hash.get_digest();
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
std::vector<std::shared_ptr<const State>> RNGInitProcess::getState() const
{
    return {getSeed()};
}
//----------------------------------------------------------------------------
boost::uuids::detail::sha1::digest_type RNGInitProcess::getMergeHashDigest() const
{
    boost::uuids::detail::sha1 hash;
    UPDATE_HASH_CLASS_NAME(RNGInitProcess);
    return hash.get_digest();
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
std::vector<std::shared_ptr<const State>> MemsetProcess::getState() const
{
    if(std::holds_alternative<Model::VariablePtr>(getTarget())) {
        return {std::get<VariablePtr>(getTarget())};
    }
    else {
        return {};
    }
}
//----------------------------------------------------------------------------
boost::uuids::detail::sha1::digest_type MemsetProcess::getMergeHashDigest() const
{
    boost::uuids::detail::sha1 hash;
    UPDATE_HASH_CLASS_NAME(MemsetProcess);
    return hash.get_digest();
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
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const State>> BroadcastProcess::getState() const
{
    std::vector<std::shared_ptr<const State>> state{getSource()};

    if(std::holds_alternative<Model::VariablePtr>(getTarget())) {
        state.push_back(std::get<VariablePtr>(getTarget()));
    }
    
    return state;
}
//----------------------------------------------------------------------------
boost::uuids::detail::sha1::digest_type BroadcastProcess::getMergeHashDigest() const
{
    boost::uuids::detail::sha1 hash;
    UPDATE_HASH_CLASS_NAME(BroadcastProcess);
    return hash.get_digest();
}
}