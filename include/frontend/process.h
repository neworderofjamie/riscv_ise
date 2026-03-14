#pragma once

// Standard C++ includes
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

// GeNN includes
#include "type.h"

// GeNN transpiler includes
#include "transpiler/token.h"

// Frontend includes
#include "frontend/event_container.h"
#include "frontend/frontend_export.h"
#include "frontend/model_component.h"
#include "frontend/shape.h"
#include "frontend/variable.h"

// Macros
#define UPDATE_HASH_CLASS_NAME(CLASS_NAME) Utils::updateHash(#CLASS_NAME, hash);

// Forward declarations
namespace Frontend
{
class Model;
class Parameter;
}

//----------------------------------------------------------------------------
// Frontend::Sliced
//----------------------------------------------------------------------------
namespace Frontend
{
template<typename T>
class Sliced
{
public:
    Sliced(std::shared_ptr<const T> underlying, bool timeSlice = false)
    :   m_Underlying(underlying), m_TimeSlice(timeSlice)
    {
        auto &shape = m_Underlying->getShape();
        if (m_TimeSlice) {
            m_Shape = shape.slice(1);
        }
        else {
            m_Shape = shape;
        }
    }

    auto getUnderlying() const{ return m_Underlying; }
    const Shape &getShape() const{ return m_Shape; }
    bool hasTimeSlice() const{ return m_TimeSlice; }
    bool hasTime() const{ return (hasTimeSlice() && (getNumTimesteps() > 1)); }
    size_t getNumTimesteps() const{ return getUnderlying()->getShape().getFirst(); }

private:
    std::shared_ptr<const T> m_Underlying;
    Shape m_Shape;
    bool m_TimeSlice;
};

using VariablePtr = std::shared_ptr<const Variable>;
using EventContainerMap = std::map<std::string, Sliced<EventContainer>>;
using VariableMap = std::map<std::string, Sliced<Variable>>;
using Literals = std::vector<std::pair<GeNN::Type::ResolvedType, GeNN::Type::NumericValue>>;


//----------------------------------------------------------------------------
// Frontend::Process
//----------------------------------------------------------------------------
class Process : public ModelComponent
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Get vector of state objects used by this process
    virtual std::vector<std::shared_ptr<const State>> getAllState() const = 0;

    //! Update the provided hash with the properties of this process which determine whether it can be merged
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const = 0;

    //! Update the compatible split dimensions of a state object (which should be
    //! one used by this process) with any constraints imposed by this process)
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const Frontend::State> state, 
                                                 uint32_t &compatibleSplitDimensions) const = 0;

    //! Once compatible split dimensions have been obtained for each state object, they might need 
    //! 'constraining' e.g. to ensure all variables associated with a neuron update process get split the same
    virtual void constrainSplitDimensions(std::unordered_map<std::shared_ptr<const Frontend::State>,
                                                             uint32_t> &compatibleSplitDimensions) const
    {
    }

protected:
    using ModelComponent::ModelComponent;
};

//----------------------------------------------------------------------------
// Frontend::NeuronUpdateProcess
//----------------------------------------------------------------------------
class FRONTEND_EXPORT NeuronUpdateProcess : public Process
{
public:
    NeuronUpdateProcess(Private, const std::string &code, const VariableMap &variables, 
                        const EventContainerMap &outputEvents, const std::string &name);

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    //! Get vector of state objects used by this process
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //! Update the provided hash with the properties of this process which determine whether it can be merged
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

    //! Update the compatible split dimensions of a state object (which should be
    //! one used by this process) with any constraints imposed by this process)
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const State> state, 
                                                 uint32_t &compatibleSplitDimensions) const override;

    //! Once compatible split dimensions have been obtained for each state object, they might need 
    //! 'constraining' e.g. to ensure all variables associated with a neuron update process get split the same
    virtual void constrainSplitDimensions(std::unordered_map<std::shared_ptr<const Frontend::State>,
                                          uint32_t> &compatibleSplitDimensions) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getVariables() const{ return m_Variables; }
    const auto &getOutputEvents() const{ return m_OutputEvents; }

    const auto &getTokens() const{ return m_Tokens; }

    const auto &getShape() const{ return m_Shape; }

    const auto &getLiterals() const{ return m_Literals; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    VariableMap m_Variables;
    EventContainerMap m_OutputEvents;

    //! Vector of literal types and numeric values built from code
    Literals m_Literals;

    //! Tokens built from code
    std::vector<GeNN::Transpiler::Token> m_Tokens;

    Shape m_Shape;
};

//----------------------------------------------------------------------------
// Frontend::EventPropagationProcess
//----------------------------------------------------------------------------
class FRONTEND_EXPORT EventPropagationProcess : public Process
{
public:
    EventPropagationProcess(Private, Sliced<EventContainer> inputEvents, 
                            VariablePtr weight, Sliced<Variable> target,
                            size_t numSparseConnectivityBits, size_t numDelayBits,
                            const std::string &name);

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    //! Get vector of state objects used by this process
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //! Update the provided hash with the properties of this process which determine whether it can be merged
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

    //! Update the compatible split dimensions of a state object (which should be
    //! one used by this process) with any constraints imposed by this process)
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const State> state, 
                                                 uint32_t &compatibleSplitDimensions) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getInputEvents() const{ return m_InputEvents; }
    const auto getWeight() const{ return m_Weight; }
    const auto getTarget() const{ return m_Target; }

    const auto &getSourceShape() const{ return m_InputEvents.getShape(); }
    const auto &getTargetShape() const{ return m_Target.getShape(); }
    size_t getMaxRowLength() const{ return m_MaxRowLength; }

    size_t getNumSparseConnectivityBits() const{ return m_NumSparseConnectivityBits; }
    size_t getNumDelayBits() const{ return m_NumDelayBits; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Sliced<EventContainer> m_InputEvents;
    VariablePtr m_Weight;
    Sliced<Variable> m_Target;
    
    size_t m_NumSourceNeurons;
    size_t m_NumTargetNeurons;
    size_t m_MaxRowLength;

    size_t m_NumSparseConnectivityBits;
    size_t m_NumDelayBits;
};

//----------------------------------------------------------------------------
// Frontend::RNGInitProcess
//----------------------------------------------------------------------------
class FRONTEND_EXPORT RNGInitProcess : public Process
{
public:
    RNGInitProcess(Private, VariablePtr seed, const std::string &name);

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    //! Get vector of state objects used by this process
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //! Update the provided hash with the properties of this process which determine whether it can be merged
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

    //! Update the compatible split dimensions of a state object (which should be
    //! one used by this process) with any constraints imposed by this process)
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const State> state, 
                                                 uint32_t &compatibleSplitDimensions) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getSeed() const{ return m_Seed; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::shared_ptr<const Variable> m_Seed;
};

//----------------------------------------------------------------------------
// Frontend::MemsetProcess
//----------------------------------------------------------------------------
//! Process for memsetting variables
class FRONTEND_EXPORT MemsetProcess : public Process
{
public:
    MemsetProcess(Private, Sliced<Variable> target, const std::string &name);

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    //! Get vector of state objects used by this process
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //! Update the provided hash with the properties of this process which determine whether it can be merged
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

    //! Update the compatible split dimensions of a state object (which should be
    //! one used by this process) with any constraints imposed by this process)
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const State> state, 
                                                 uint32_t &compatibleSplitDimensions) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getTarget() const{ return m_Target; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Sliced<Variable> m_Target;
};

}
