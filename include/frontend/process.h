#pragma once

// Standard C++ includes
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

// Common include
#include "common/utils.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"
#include "compiler_frontend/token.h"

// Frontend includes
#include "frontend/events.h"
#include "frontend/frontend_export.h"
#include "frontend/model_component.h"
#include "frontend/shape.h"
#include "frontend/variable.h"

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

    void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const
    {
        ::Common::Utils::updateHash(hasTime(), hash);
        if (hasTime()) {
            ::Common::Utils::updateHash(getNumTimesteps(), hash);
        }
        m_Underlying->updateMergeHash(hash, model);
    }
private:
    std::shared_ptr<const T> m_Underlying;
    Shape m_Shape;
    bool m_TimeSlice;
};

using VariablePtr = std::shared_ptr<const Variable>;
using EventSinkMap = std::map<std::string, Sliced<EventSink>>;
using VariableMap = std::map<std::string, Sliced<Variable>>;
using Literals = std::vector<std::tuple<CompilerFrontend::Type::ResolvedType, 
                                        CompilerFrontend::Type::NumericValue, size_t>>;


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

    //! Get vector of event source objects used by this process
    virtual std::vector<std::shared_ptr<const EventSource>> getAllEventSources() const = 0;

    //! Get vector of event sink objects used by this process
    virtual std::vector<Sliced<EventSink>> getAllEventSinks() const = 0;

    //! Update the provided hash with the properties of this process which determine whether it can be merged
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const = 0;

    //! Update the compatible split dimensions of a state object (which should be
    //! one used by this process) with any constraints imposed by this process)
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const Frontend::State> state, 
                                                 uint32_t &compatibleSplitDimensions) const = 0;

    //! Once compatible split dimensions have been obtained for each state object, they might need 
    //! 'constraining' e.g. to ensure all variables associated with a neuron update process get split the same
    virtual void constrainSplitDimensions(std::unordered_map<std::shared_ptr<const Frontend::State>, uint32_t> &) const
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
                        const EventSinkMap &outputEventSinks, 
                        const CompilerFrontend::Type::ResolvedType &defaultScalarLiteralType,
                        const std::string &name);

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    //! Get vector of state objects used by this process
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //! Get vector of event source objects used by this process
    virtual std::vector<std::shared_ptr<const EventSource>> getAllEventSources() const override final;

    //! Get vector of event sink objects used by this process
    virtual std::vector<Sliced<EventSink>> getAllEventSinks() const override final;

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
    const auto &getOutputEventSinks() const{ return m_OutputEventSinks; }

    const auto &getTokens() const{ return m_Tokens; }

    const auto &getShape() const{ return m_Shape; }

    const auto &getLiterals() const{ return m_Literals; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<NeuronUpdateProcess> create(
        const std::string &code, const VariableMap &variables = {}, const EventSinkMap &outputEventSinks = {}, 
        const CompilerFrontend::Type::ResolvedType &defaultScalarLiteralType = CompilerFrontend::Type::Float,
        const std::string &name = "")
    {
        return std::make_shared<NeuronUpdateProcess>(Private(), code, variables, outputEventSinks, 
                                                     defaultScalarLiteralType, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    VariableMap m_Variables;
    EventSinkMap m_OutputEventSinks;

    //! Vector of literal types and numeric values built from code
    Literals m_Literals;

    //! Tokens built from code
    std::vector<CompilerFrontend::Token> m_Tokens;
    
    Shape m_Shape;
};

//----------------------------------------------------------------------------
// Frontend::EventPropagationProcess
//----------------------------------------------------------------------------
class FRONTEND_EXPORT EventPropagationProcess : public Process
{
public:
    EventPropagationProcess(Private, Sliced<EventSource> inputEventSource, 
                            Sliced<Variable> target, const std::string &name);

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    //! Get vector of state objects used by this process
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override;

    //! Get vector of event source objects used by this process
    virtual std::vector<std::shared_ptr<const EventSource>> getAllEventSources() const override final;
    
    //! Get vector of event sink objects used by this process
    virtual std::vector<Sliced<EventSink>> getAllEventSinks() const override final;

    //! Update the compatible split dimensions of a state object (which should be
    //! one used by this process) with any constraints imposed by this process)
    virtual void updateCompatibleSplitDimensions(std::shared_ptr<const State> state, 
                                                 uint32_t &compatibleSplitDimensions) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getInputEventSource() const{ return m_InputEventSource; }
    const auto getTarget() const{ return m_Target; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventPropagationProcess> create(std::shared_ptr<const EventSource> inputEventSource, 
                                                           VariablePtr target, const std::string &name = "")
    {
        return std::make_shared<EventPropagationProcess>(Private(), inputEventSource, target, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Sliced<EventSource> m_InputEventSource;

    Sliced<Variable> m_Target;
    
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

    //! Get vector of event source objects used by this process
    virtual std::vector<std::shared_ptr<const EventSource>> getAllEventSources() const override final;

    //! Get vector of event sink objects used by this process
    virtual std::vector<Sliced<EventSink>> getAllEventSinks() const override final;

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

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<RNGInitProcess> create(VariablePtr seed, const std::string &name = "")
    {
        return std::make_shared<RNGInitProcess>(Private(), seed, name);
    }
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

    //! Get vector of event source objects used by this process
    virtual std::vector<std::shared_ptr<const EventSource>> getAllEventSources() const override final;

    //! Get vector of event sink objects used by this process
    virtual std::vector<Sliced<EventSink>> getAllEventSinks() const override final;

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

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<MemsetProcess> create(VariablePtr target, const std::string &name = "")
    {
        return std::make_shared<MemsetProcess>(Private(), target, name);
    }
private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Sliced<Variable> m_Target;
};
}
