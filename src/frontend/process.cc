#include "frontend/process.h"

// Standard C++ includes
#include <stdexcept>

// Fast float includes
#include <fast_float/fast_float.h>

// Compiler frontend includes
#include "compiler_frontend/error_handler.h"
#include "compiler_frontend/scanner.h"

// Compiler includes
#include "frontend/events.h"
#include "frontend/variable.h"

//----------------------------------------------------------------------------
// Frontend::NeuronUpdateProcess
//----------------------------------------------------------------------------
namespace Frontend
{
NeuronUpdateProcess::NeuronUpdateProcess(Private, const std::string &code, const VariableMap &variables, 
                                         const EventSinkMap &outputEventSinks, 
                                         const CompilerFrontend::Type::ResolvedType &defaultScalarLiteralType,
                                         const std::string &name)
:   Process(name), m_Variables(variables), m_OutputEventSinks(outputEventSinks)
{
    if(m_Variables.empty() && m_OutputEventSinks.empty()) {
        throw std::runtime_error("Neuron update process requires at least one variable or output event sink");
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
        auto firstNonScalarEvent = std::find_if(getOutputEventSinks().cbegin(), getOutputEventSinks().cend(),
                                                [](const auto &e){ return !e.second.getShape().isScalar(); });
        if(firstNonScalarEvent != getOutputEventSinks().cend()) {
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
    for(const auto &o : m_OutputEventSinks) {
        if(o.second.getShape() != m_Shape) {
            throw std::runtime_error("Output events '" + o.first + "' with shape: " + o.second.getShape().toString()
                                     + " is not compatible with neuron update process with shape: " + m_Shape.toString());
        }
    }
    
    // Scan code string and return tokens
    CompilerFrontend::ErrorHandler errorHandler("NeuronUpdateProcess '" + getName() + "'");
    m_Tokens = CompilerFrontend::Scanner::scanSource(code, errorHandler, 
                                                     defaultScalarLiteralType);
    if(errorHandler.hasError()) {
        throw std::runtime_error("Error scanning");
    }

    // Loop through tokens
    for(const auto &t: getTokens()) {
        // If this token is a numeric literal,
        if (t.type == CompilerFrontend::Token::Type::NUMBER) {
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
                        m_Literals.emplace_back(t.numberType.value(), result, t.index);
                    }
                    else {
                        throw std::runtime_error("Unable to pass numeric literal '" + lexeme + "'");
                    }
                    
                }
                else {
                    uint64_t result;
                    auto answer = fast_float::from_chars(lexemeBegin, lexemeEnd, result);
                    if(answer.ec == std::errc()) {
                        m_Literals.emplace_back(t.numberType.value(), result, t.index);
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
                    m_Literals.emplace_back(t.numberType.value(), result, t.index);
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
    std::transform(getOutputEventSinks().cbegin(), getOutputEventSinks().cend(), std::back_inserter(state),
                   [](const auto &o){ return o.second.getUnderlying(); });
    return state;
}
//----------------------------------------------------------------------------
void NeuronUpdateProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    using namespace Common;

    UPDATE_HASH_CLASS_NAME(NeuronUpdateProcess);

    // Variables
    Utils::updateHash(getVariables().size(), hash);
    for(const auto &v : getVariables()) {
        Utils::updateHash(v.first, hash);
        v.second.getUnderlying()->updateMergeHash(hash);
    }

    // Output events
    Utils::updateHash(getOutputEventSinks().size(), hash);
    for(const auto &e : getOutputEventSinks()) {
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
        if (t.type != CompilerFrontend::Token::Type::NUMBER) {
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
        const auto outEvent = std::find_if(getOutputEventSinks().cbegin(), getOutputEventSinks().cend(),
                                           [&state](const auto &o){ return o.second.getUnderlying() == state; });
        if (outEvent != getOutputEventSinks().cend()) {
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
void NeuronUpdateProcess::constrainSplitDimensions(std::unordered_map<std::shared_ptr<const Frontend::State>,
                                                                      uint32_t> &compatibleSplitDimensions) const
{
    // AND together all variable split dimensions
    uint32_t combinedSplitDimensions = 0xFFFFFFFFu;
    for(const auto &v : getVariables()) {
        combinedSplitDimensions &= compatibleSplitDimensions.at(v.second.getUnderlying());
    }

    // AND output event split dimension
    for(const auto &o : getOutputEventSinks()) {
        combinedSplitDimensions &= compatibleSplitDimensions.at(o.second.getUnderlying());
    }

    // Update all variables compatible split dimensions with the combined version
    for(const auto &v : getVariables()) {
        compatibleSplitDimensions.at(v.second.getUnderlying()) = combinedSplitDimensions;
    }

    // Update all output events compatible split dimensions with the combined version
    for(const auto &o : getOutputEventSinks()) {
        compatibleSplitDimensions.at(o.second.getUnderlying()) = combinedSplitDimensions;
    }
}
//----------------------------------------------------------------------------
// EventPropagationProcess
//----------------------------------------------------------------------------
EventPropagationProcess::EventPropagationProcess(Private, Sliced<EventSource> inputEventSource, 
                                                 Sliced<Variable> target, const std::string &name)
:   Process(name), m_InputEventSource(inputEventSource),  m_Target(target)
{
    if(getInputEventSource().getUnderlying() == nullptr) {
        throw std::runtime_error("Event propagation process requires input events");
    }

    if(getTarget().getUnderlying() == nullptr) {
        throw std::runtime_error("Event propagation process requires target variable");
    }

    if (getSourceShape().getNumDims() != 1) {
        throw std::runtime_error("Event propagation process requires source events with a 1D shape");
    }    
}
//----------------------------------------------------------------------------
std::vector<std::shared_ptr<const State>> EventPropagationProcess::getAllState() const
{
    return {getInputEventSource().getUnderlying(), getTarget().getUnderlying()};
}
//----------------------------------------------------------------------------
void EventPropagationProcess::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    using namespace Common::Utils;
    UPDATE_HASH_CLASS_NAME(EventPropagationProcess);

    // Input events
    getInputEventSource().getUnderlying()->updateMergeHash(hash);

    // Targets
    getTarget().getUnderlying()->updateMergeHash(hash);
}
//----------------------------------------------------------------------------
void EventPropagationProcess::updateCompatibleSplitDimensions(std::shared_ptr<const State> state, 
                                                              uint32_t &compatibleSplitDimensions) const 
{
    // If variable's target
    if(state == getTarget().getUnderlying()) {
        // If there are no delays, it can only be split on 1st (postsynaptic) dimension
        if (getTarget().getShape().getNumDims() == 1) {
            compatibleSplitDimensions &= (1 << 0);
        }
        // Otherwise, it can only be split on 2nd (postsynaptic) dimension
        else {
            compatibleSplitDimensions &= (1 << 1);
        }
    }
    // Otherwise, if it's input event container, we should receive all splits
    else {
        assert(state == getInputEventSource().getUnderlying());
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
