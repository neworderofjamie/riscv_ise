#pragma once

// Standard C++ includes
#include <memory>
#include <string>

// Boost includes
#include <sha1.hpp>

// Forward declarations
namespace Frontend
{
class Shape;
}

//----------------------------------------------------------------------------
// Frontend::ModelComponent
//----------------------------------------------------------------------------
namespace Frontend
{
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
// Frontend::State
//----------------------------------------------------------------------------
class StateVisitor
{
public:
    virtual void visit(std::shared_ptr<const EventContainer>){}
    virtual void visit(std::shared_ptr<const Variable>){}
};

//----------------------------------------------------------------------------
// Frontend::State
//----------------------------------------------------------------------------
class State : public ModelComponent
{
public:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual void accept(StateVisitor &visitor) const = 0;
    virtual const Shape &getShape() const = 0;

protected:
    using ModelComponent::ModelComponent;
};

//---------------------------------------------------------------------------
// Frontend::AcceptableState
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
}
