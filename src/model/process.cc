// Standard C++ includes
#include <stdexcept>

// Fast float includes
#include <fast_float/fast_float.h>

// GeNN includes
#include "gennUtils.h"

// Compiler includes
#include "model/event_container.h"
#include "model/process.h"
#include "model/variable.h"

#define UPDATE_HASH_CLASS_NAME(CLASS_NAME) GeNN::Utils::updateHash(#CLASS_NAME, hash);

//----------------------------------------------------------------------------
// Model::NeuronUpdateProcess
//----------------------------------------------------------------------------
namespace Model
{
NeuronUpdateProcess::NeuronUpdateProcess(Private, const std::string &code, const VariableMap &variables, 
                                         const EventContainerMap &outputEvents, const std::string &name)
:   Process(name), m_Variables(variables), m_OutputEvents(outputEvents)
{
    if(m_Variables.empty() && m_OutputEvents.empty()) {
        throw std::runtime_error("Neuron update process requires at least one variable or output event");
    }

    // If one of the variables has a non-scalar shape, use that
    auto firstNonScalarVar = std::find_if(getVariables().cbegin(), getVariables().cend(),
                                          [](const auto &v){ return !v.second.getShape().isScalar(); });
    if(firstNonScalarVar != getVariables().cend()) {
        m_Shape = firstNonScalarVar->second.getShape();
    }
    // Otherwise
    else {
        // If one of the output event containers has a non-scalar shape, use that
        auto firstNonScalarEvent = std::find_if(getOutputEvents().cbegin(), getOutputEvents().cend(),
                                                [](const auto &e){ return !e.second.getShape().isScalar(); });
        if(firstNonScalarEvent != getOutputEvents().cend()) {
            m_Shape = firstNonScalarEvent->second.getShape();
        }
        // Otherwise, shape really must be scalar!
        else {
            m_Shape = Shape(1);
        }
    }
   

    // Check all variables have same number of neurons
    for(const auto &v : m_Variables) {
        if(v.second.getShape() != m_Shape) {
            throw std::runtime_error("Variable '" + v.first + "' with shape: " + v.second.getShape().toString() 
                                     + " is not compatible with neuron update process with shape: " + m_Shape.toString());
        }
    }

    // Check all output have same number of neurons
    for(const auto &o : m_OutputEvents) {
        if(o.second.getShape() != m_Shape) {
            throw std::runtime_error("Output events '" + o.first + "' with shape: " + o.second.getShape().toString()
                                     + " is not compatible with neuron update process with shape: " + m_Shape.toString());
        }
    }
    
    // Parse code
    m_Tokens = GeNN::Utils::scanCode(code, "NeuronUpdateProcess");

    // Loop through tokens
    for(const auto &t: getTokens()) {
        // If this token is a numeric literal,
        if (t.type == GeNN::Transpiler::Token::Type::NUMBER) {
            // Get start and end of lexeme
            const auto lexeme = t.lexeme;
            const char *lexemeBegin = lexeme.c_str();
            const char *lexemeEnd = lexemeBegin + lexeme.size();

            // Get it's type (scalar if not specified)
            const auto &numericType = t.numberType.value().getNumeric();
            if(numericType.isIntegral) {
                if(numericType.isSigned) {
                    int64_t result;
                    auto answer = fast_float::from_chars(lexemeBegin, lexemeEnd, result);
                    if(answer.ec == std::errc()) {
                        m_Literals.emplace(t.numberType.value(), result);
                    }
                    else {
                        throw std::runtime_error("Unable to pass numeric literal '" + lexeme + "'");
                    }
                    
                }
                else {
                    uint64_t result;
                    auto answer = fast_float::from_chars(lexemeBegin, lexemeEnd, result);
                    if(answer.ec == std::errc()) {
                        m_Literals.emplace(t.numberType.value(), result);
                    }
                    else {
                        throw std::runtime_error("Unable to pass numeric literal '" + lexeme + "'");
                    }
                }
            }
            // Otherwise, if it is fixed point or floating point
            else {
                double result;
                auto answer = fast_float::from_chars(lexemeBegin, lexemeEnd, result);
                if(answer.ec == std::errc()) {
                    m_Literals.emplace(t.numberType.value(), result);
                }
                else {
                    throw std::runtime_error("Unable to pass numeric literal '" + lexeme + "'");
                }
            }
        }
    }
    // Batched = any output events batched
    // 
}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const State>> NeuronUpdateProcess::getAllState() const
{
    std::vector<std::shared_ptr<const State>> state;
    std::transform(getVariables().cbegin(), getVariables().cend(), std::back_inserter(state),
                   [](const auto &v){ return v.second.getUnderlying(); });
    std::transform(getOutputEvents().cbegin(), getOutputEvents().cend(), std::back_inserter(state),
                   [](const auto &o){ return o.second.getUnderlying(); });
    return state;
}
//----------------------------------------------------------------------------
void NeuronUpdateProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    using namespace GeNN::Utils;
    UPDATE_HASH_CLASS_NAME(NeuronUpdateProcess);

    // Variables
    updateHash(getVariables().size(), hash);
    for(const auto &v : getVariables()) {
        updateHash(v.first, hash);
        v.second.getUnderlying()->updateMergeHash(hash);
    }

    // Output events
    updateHash(getOutputEvents().size(), hash);
    for(const auto &e : getOutputEvents()) {
        updateHash(e.first, hash);
        e.second.getUnderlying()->updateMergeHash(hash);
    }

    // Tokens
    updateHash(getTokens().size(), hash);
    for(const auto &t : getTokens()) {
        updateHash(t.type, hash);
        updateHash(t.numberType, hash);

        // If this is a numeric literal, don't include it's value
        // Literals can vary between merged processes
        if (t.type != GeNN::Transpiler::Token::Type::NUMBER) {
            updateHash(t.lexeme, hash);
        }
    }
}

//----------------------------------------------------------------------------
// EventPropagationProcess
//----------------------------------------------------------------------------
EventPropagationProcess::EventPropagationProcess(Private, Sliced<EventContainer> inputEvents, 
                                                 VariablePtr weight, Sliced<Variable> target, size_t numSparseConnectivityBits, 
                                                 size_t numDelayBits, const std::string &name)
:   Process(name), m_InputEvents(inputEvents), m_Weight(weight), m_Target(target), 
    m_NumSparseConnectivityBits(numSparseConnectivityBits), m_NumDelayBits(numDelayBits)
{
    if(m_InputEvents.getUnderlying() == nullptr) {
        throw std::runtime_error("Event propagation process requires input events");
    }

    if(m_Weight == nullptr) {
        throw std::runtime_error("Event propagation process requires weight variable");
    }

    if(m_Target.getUnderlying() == nullptr) {
        throw std::runtime_error("Event propagation process requires target variable");
    }

    if (getSourceShape().getDims().size() != 1) {
        throw std::runtime_error("Event propagation process requires source events with a 1D shape");
    }

    if (getWeight()->getShape().getDims().size() != 2) {
        throw std::runtime_error("Event propagation process requires weight variable with a 2D shape");
    }

  
    // Get maximum row length from weight variable shape
    const auto &weightDims = m_Weight->getShape().getDims();
    m_MaxRowLength = weightDims[1];

    // Check weight number of source neurons matches
    if(weightDims[0] != getSourceShape().getLast()) {
        throw std::runtime_error("Weight with shape: " + weight->getShape().toString() 
                                 + " is not compatible with event propagation process with " 
                                 + std::to_string(getSourceShape().getLast()) + " source neurons");
    }

    // Check delays and sparsity are not being combined
    if(m_NumDelayBits > 0 && m_NumSparseConnectivityBits > 0) {
        throw std::runtime_error("Event propagation processes with both events "
                                 "and delays are not currently supported");
    }

    // Check weight number of target neurons matches if no sparsity
    if(m_NumSparseConnectivityBits == 0 && m_MaxRowLength != getTargetShape().getLast()) {
        throw std::runtime_error("Weight with shape: " + weight->getShape().toString() 
                                 + " is not compatible with dense event propagation process with " 
                                 + std::to_string(getTargetShape().getLast()) + " target neurons");
    }

    // If there are no delays, check time 
    if (m_NumDelayBits == 0) {
        if (getTargetShape().getDims().size() != 1) {
            throw std::runtime_error("Non-delayed event propagation process "
                                     "requires target variable with a 1D shape");
        }
    }
    // Otherwise, check buffer size matches
    else {
        if (getTargetShape().getDims().size() != 2) {
            throw std::runtime_error("Delayed event propagation process "
                                     "requires target variable with a 2D shape");
        }

        if(getTargetShape().getFirst() != (1 << (m_NumDelayBits - 1))) {
            throw std::runtime_error("Shape of target buffer does not "
                                     "match specified number of delay bits");
        }
    }
    
}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const State>> EventPropagationProcess::getAllState() const
{
    return {getInputEvents().getUnderlying(), getWeight(), getTarget().getUnderlying()};
}
//----------------------------------------------------------------------------
void EventPropagationProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    using namespace GeNN::Utils;
    UPDATE_HASH_CLASS_NAME(EventPropagationProcess);

    // Input events
    getInputEvents().getUnderlying()->updateMergeHash(hash);

    // Weights and targets
    getWeight()->updateMergeHash(hash);
    getTarget().getUnderlying()->updateMergeHash(hash);

    // Formats
    updateHash(getNumDelayBits(), hash);
    updateHash(getNumSparseConnectivityBits(), hash);
}

//----------------------------------------------------------------------------
// RNGInitProcess
//----------------------------------------------------------------------------
RNGInitProcess::RNGInitProcess(Private, VariablePtr seed, const std::string &name)
:   Process(name), m_Seed(seed)
{
    if(m_Seed == nullptr) {
        throw std::runtime_error("RNG init process requires seed");
    }
}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const State>> RNGInitProcess::getAllState() const
{
    return {getSeed()};
}
//----------------------------------------------------------------------------
void RNGInitProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    UPDATE_HASH_CLASS_NAME(RNGInitProcess);
}

//----------------------------------------------------------------------------
// MemsetProcess
//----------------------------------------------------------------------------
MemsetProcess::MemsetProcess(Private, Sliced<Variable> target, const std::string &name)
:   Process(name), m_Target(target)
{
    // If target is a variable
    // **NOTE** if target is a backend-specific state object, there's nothing we can check yet
    //if(std::holds_alternative<VariablePtr>(m_Target)) {
        //auto targetVar = std::get<VariablePtr>(m_Target);
        if(getTarget().getUnderlying() == nullptr) {
            throw std::runtime_error("Memset process requires target");
        }
    //}
}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const State>> MemsetProcess::getAllState() const
{
    //if(std::holds_alternative<VariablePtr>(getTarget())) {
        return {getTarget().getUnderlying()};
    //}
    //else {
    //    return {};
    //}
}
//----------------------------------------------------------------------------
void MemsetProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    UPDATE_HASH_CLASS_NAME(MemsetProcess);
}

//----------------------------------------------------------------------------
// BroadcastProcess
//----------------------------------------------------------------------------
BroadcastProcess::BroadcastProcess(Private, VariablePtr source, VariablePtr target, const std::string &name)
:   Process(name), m_Source(source), m_Target(target)
{
    if(m_Source == nullptr) {
        throw std::runtime_error("Broadcast process requires source");
    }

    if(m_Source->getShape().getDims().size() != 1) {
        throw std::runtime_error("Multi-dimensional sources aren't currently "
                                 "supported by broadcast processes");
    }

    // If target is a variable
    if(m_Target == nullptr) {
        throw std::runtime_error("Broadcast process requires target");
    }

    if(m_Target->getShape().getDims().size() != 2) {
        throw std::runtime_error("Broadcast process currently required 2 dimensional target");
    }

    if(m_Target->getShape().getFirst() != m_Source->getShape().getFirst()) {
        throw std::runtime_error("Broadcast process requires first dimension of source and target to match");
    }

    if (m_Source->getType() != m_Target->getType()) {
        throw std::runtime_error("Broadcast process requires source and target with same shape");
    }
}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const State>> BroadcastProcess::getAllState() const
{
    return {getSource(), getTarget()};
}
//----------------------------------------------------------------------------
void BroadcastProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    UPDATE_HASH_CLASS_NAME(BroadcastProcess);
}
}