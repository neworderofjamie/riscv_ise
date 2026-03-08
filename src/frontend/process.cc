#include "frontend/process.h"

// Standard C++ includes
#include <stdexcept>

// Fast float includes
#include <fast_float/fast_float.h>

// GeNN includes
#include "gennUtils.h"

// Compiler includes
#include "frontend/event_container.h"
#include "frontend/variable.h"

using namespace GeNN;

//----------------------------------------------------------------------------
// Frontend::NeuronUpdateProcess
//----------------------------------------------------------------------------
namespace Frontend
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
                        m_Literals.emplace_back(t.numberType.value(), result);
                    }
                    else {
                        throw std::runtime_error("Unable to pass numeric literal '" + lexeme + "'");
                    }
                    
                }
                else {
                    uint64_t result;
                    auto answer = fast_float::from_chars(lexemeBegin, lexemeEnd, result);
                    if(answer.ec == std::errc()) {
                        m_Literals.emplace_back(t.numberType.value(), result);
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
                    m_Literals.emplace_back(t.numberType.value(), result);
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
    UPDATE_HASH_CLASS_NAME(NeuronUpdateProcess);

    // Variables
    Utils::updateHash(getVariables().size(), hash);
    for(const auto &v : getVariables()) {
        Utils::updateHash(v.first, hash);
        v.second.getUnderlying()->updateMergeHash(hash);
    }

    // Output events
    Utils::updateHash(getOutputEvents().size(), hash);
    for(const auto &e : getOutputEvents()) {
        Utils::updateHash(e.first, hash);
        e.second.getUnderlying()->updateMergeHash(hash);
    }

    // Tokens
    Utils::updateHash(getTokens().size(), hash);
    for(const auto &t : getTokens()) {
        Utils::updateHash(t.type, hash);
        Utils::updateHash(t.numberType, hash);

        // If this is a numeric literal, don't include it's value
        // Literals can vary between merged processes
        if (t.type != GeNN::Transpiler::Token::Type::NUMBER) {
            Utils::updateHash(t.lexeme, hash);
        }
    }
}
//----------------------------------------------------------------------------
void NeuronUpdateProcess::updateCompatibleSplitDimensions(std::shared_ptr<const State> state, 
                                                          uint32_t &compatibleSplitDimensions) const 
{
    // If state is a variable
    const auto var = std::find_if(getVariables().cbegin(), getVariables().cend(),
                                  [&state](const auto &v){ return v.second.getUnderlying() == state; });
    if (var != getVariables().cend()) {
        // Ensure that we only split along the dimensions of the slice  
        // exposed to the neuron update process i.e. not the time dimension
        compatibleSplitDimensions &= ((1 << var->second.getShape().getNumDims()) - 1);
    }
    // Otherwise
    else {
        // If state is an output event
        const auto outEvent = std::find_if(getOutputEvents().cbegin(), getOutputEvents().cend(),
                                           [&state](const auto &o){ return o.second.getUnderlying() == state; });
        if (outEvent != getOutputEvents().cend()) {
            // Ensure that we only split along the dimensions of the slice  
            // exposed to the neuron update process i.e. not the time dimension
            compatibleSplitDimensions &= ((1 << outEvent->second.getShape().getNumDims()) - 1);
        }
        else {
            assert(false);
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

    if (getSourceShape().getNumDims() != 1) {
        throw std::runtime_error("Event propagation process requires source events with a 1D shape");
    }

    if (getWeight()->getShape().getNumDims() != 2) {
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
    if(getNumDelayBits() > 0 && getNumSparseConnectivityBits() > 0) {
        throw std::runtime_error("Event propagation processes with both events "
                                 "and delays are not currently supported");
    }

    // Check weight number of target neurons matches if no sparsity
    if(getNumSparseConnectivityBits() == 0 && getMaxRowLength() != getTargetShape().getLast()) {
        throw std::runtime_error("Weight with shape: " + weight->getShape().toString() 
                                 + " is not compatible with dense event propagation process with " 
                                 + std::to_string(getTargetShape().getLast()) + " target neurons");
    }

    // If there are no delays, check time 
    if (getNumDelayBits() == 0) {
        if (getTargetShape().getNumDims() != 1) {
            throw std::runtime_error("Non-delayed event propagation process "
                                     "requires target variable with a 1D shape");
        }
    }
    // Otherwise, check buffer size matches
    else {
        if (getTargetShape().getNumDims() != 2 || m_Target.hasTimeSlice()) {
            throw std::runtime_error("Delayed event propagation process "
                                     "requires target variable with a 2D shape");
        }

        if(getTargetShape().getFirst() != (1 << (getNumDelayBits() - 1))) {
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
void EventPropagationProcess::updateCompatibleSplitDimensions(std::shared_ptr<const State> state, 
                                                              uint32_t &compatibleSplitDimensions) const 
{
    // If variable is weight, it can only be split in 2nd (postsynaptic) dimension
    if(state == getWeight()) {
        compatibleSplitDimensions &= (1 << 1);
    }
    // Otherwise, if variable's target
    else if(state == getTarget().getUnderlying()) {
        // If there are no delays, it can only be split on 1st (postsynaptic) dimension
        if (getNumDelayBits() == 0) {
            compatibleSplitDimensions &= (1 << 0);
        }
        // Otherwise, it can only be split on 2nd (postsynaptic) dimension
        else {
            compatibleSplitDimensions &= (1 << 1);
        }
    }
    // Otherwise, if it's input event container, it can't be split at all
    else {
        assert(state == getInputEvents().getUnderlying());
        compatibleSplitDimensions = 0;
    }
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
void RNGInitProcess::updateCompatibleSplitDimensions(std::shared_ptr<const State> state, 
                                                     uint32_t &compatibleSplitDimensions) const
{
    assert(state == getSeed());

    // Seed should be split along device axis
    compatibleSplitDimensions &= (1 << 1);
}

//----------------------------------------------------------------------------
// MemsetProcess
//----------------------------------------------------------------------------
MemsetProcess::MemsetProcess(Private, Sliced<Variable> target, const std::string &name)
:   Process(name), m_Target(target)
{
    if(getTarget().getUnderlying() == nullptr) {
        throw std::runtime_error("Memset process requires target");
    }
}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const State>> MemsetProcess::getAllState() const
{
    return {getTarget().getUnderlying()};
}
//----------------------------------------------------------------------------
void MemsetProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    UPDATE_HASH_CLASS_NAME(MemsetProcess);
}
//----------------------------------------------------------------------------
void MemsetProcess::updateCompatibleSplitDimensions(std::shared_ptr<const State> state, 
                                                    uint32_t &compatibleSplitDimensions) const
{
    assert(state == getTarget().getUnderlying());
    
    // Ensure that we only split along the dimensions of the slice  
    // exposed to the memset process i.e. not the time dimension
    compatibleSplitDimensions &= ((1 << getTarget().getShape().getNumDims()) - 1);
}
}
