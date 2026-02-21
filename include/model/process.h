#pragma once

// Standard C++ includes
#include <map>
#include <memory>
#include <set>
#include <string>
#include <variant>

// GeNN includes
#include "type.h"

// GeNN transpiler includes
#include "transpiler/token.h"

// Model includes
#include "model/model_export.h"
#include "model/model_component.h"
#include "model/shape.h"

// Forward declarations
namespace Model
{
class EventContainer;
class Parameter;
class Variable;
}

namespace Model
{
//----------------------------------------------------------------------------
// Model::Sliced
//----------------------------------------------------------------------------
template<typename T>
class Sliced
{
public:
    Sliced(std::shared_ptr<const T> underlying, bool timeSlice = false)
    :   m_Underlying(underlying), m_TimeSlice(timeSlice)
    {}

    auto getUnderlying() const{ return m_Underlying; }
    Shape getShape() const
    {
        auto &shape = m_Underlying->getShape();
        if (m_TimeSlice) {
            const auto &dims = shape.getDims();
            if (dims.size() == 1) {
                return Shape(1);
            }
            else {
                std::vector<size_t> slicedDims(dims.cbegin() + 1, dims.cend());
                return Shape(slicedDims);
            }
        }
        else {
            return shape;
        }
    }

private:
    std::shared_ptr<const T> m_Underlying;
    bool m_TimeSlice;
};

using VariablePtr = std::shared_ptr<const Variable>;
using VariablePtrBackendState = std::variant<VariablePtr, int>;
using EventContainerMap = std::map<std::string, Sliced<EventContainer>>;
using VariableMap = std::map<std::string, Sliced<Variable>>;
using LiteralSet = std::set<std::pair<GeNN::Type::ResolvedType, GeNN::Type::NumericValue>>;

//----------------------------------------------------------------------------
// Model::NeuronUpdateProcess
//----------------------------------------------------------------------------
class MODEL_EXPORT NeuronUpdateProcess : public Process
{
public:
    NeuronUpdateProcess(Private, const std::string &code, const VariableMap &variables, 
                        const EventContainerMap &outputEvents, const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

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

    //! Set of literal types and numeric values built from code
    LiteralSet m_Literals;

    //! Tokens built from code
    std::vector<GeNN::Transpiler::Token> m_Tokens;

    Shape m_Shape;
};

//----------------------------------------------------------------------------
// Model::EventPropagationProcess
//----------------------------------------------------------------------------
class MODEL_EXPORT EventPropagationProcess : public Process
{
public:
    EventPropagationProcess(Private, Sliced<EventContainer> inputEvents, 
                            VariablePtr weight, Sliced<Variable> target,
                            size_t numSparseConnectivityBits, size_t numDelayBits,
                            const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

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
// Model::RNGInitProcess
//----------------------------------------------------------------------------
class MODEL_EXPORT RNGInitProcess : public Process
{
public:
    RNGInitProcess(Private, VariablePtr seed, const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

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
// Model::MemsetProcess
//----------------------------------------------------------------------------
//! Process for memsetting variables
class MODEL_EXPORT MemsetProcess : public Process
{
public:
    MemsetProcess(Private, Sliced<Variable> target, const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

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

//----------------------------------------------------------------------------
// Model::BroadcastProcess
//----------------------------------------------------------------------------
//! Process for 'broadcasting' one scalar array across multiple vector lanes
class MODEL_EXPORT BroadcastProcess : public Process
{
public:
    BroadcastProcess(Private, VariablePtr source, VariablePtrBackendState target, const std::string &name);

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Process virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto getSource() const{ return m_Source; }
    const auto getTarget() const{ return m_Target; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    VariablePtr m_Source;
    VariablePtrBackendState m_Target;
};
}