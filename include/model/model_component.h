#pragma once

// Standard C++ includes
#include <memory>
#include <string>

// Boost includes
#include <sha1.hpp>

// Forward declarations
namespace Model
{
class EventContainer;
class Model;
class PerformanceCounter;
class Variable;
}

//----------------------------------------------------------------------------
// Model::ModelComponent
//----------------------------------------------------------------------------
namespace Model
{
//----------------------------------------------------------------------------
// Model::ModelComponent
//----------------------------------------------------------------------------
//! Base class for things that can be added to models
class ModelComponent : public std::enable_shared_from_this<ModelComponent>
{
public:
    ModelComponent(const ModelComponent&) = delete;
    virtual ~ModelComponent() = default;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const std::string &getName() const{ return m_Name; }

protected:
    struct Private{ explicit Private() = default; };

    ModelComponent(const std::string &name) : m_Name(name)
    {}

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::string m_Name;
};

//----------------------------------------------------------------------------
// Model::State
//----------------------------------------------------------------------------
class StateVisitor
{
public:
    virtual void visit(std::shared_ptr<const EventContainer>){}
    virtual void visit(std::shared_ptr<const PerformanceCounter>){}
    virtual void visit(std::shared_ptr<const Variable>){}
};

//----------------------------------------------------------------------------
// Model::State
//----------------------------------------------------------------------------
class State : public ModelComponent
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual void accept(StateVisitor &visitor) const = 0;

protected:
    using ModelComponent::ModelComponent;
};

//---------------------------------------------------------------------------
// AcceptableState
//---------------------------------------------------------------------------
template<typename T>
class AcceptableState : public State
{
public:
    virtual void accept(StateVisitor &visitor) const final
    {
        visitor.visit(std::static_pointer_cast<const T>(this->shared_from_this()));
    }

protected:
    using State::State;
};

//----------------------------------------------------------------------------
// Model::Stateful
//----------------------------------------------------------------------------
class Stateful : public ModelComponent
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const = 0;

protected:
    using ModelComponent::ModelComponent;
};


//----------------------------------------------------------------------------
// Model::Process
//----------------------------------------------------------------------------
class Process : public Stateful
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash, const Model &model) const = 0;

protected:
    using Stateful::Stateful;
};
}