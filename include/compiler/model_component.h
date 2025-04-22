#pragma once

// Standard C++ includes
#include <memory>
#include <string>

// Forward declarations
class EventContainer;
class Parameter;
class ProcessGroup;
class NeuronUpdateProcess;
class EventPropagationProcess;
class RNGInitProcess;
class Variable;

//----------------------------------------------------------------------------
// ModelComponent
//----------------------------------------------------------------------------
class ModelComponentVisitor
{
public:
    virtual void visit(std::shared_ptr<const EventContainer>){}
    virtual void visit(std::shared_ptr<const Parameter>){}
    virtual void visit(std::shared_ptr<const ProcessGroup>){}
    virtual void visit(std::shared_ptr<const NeuronUpdateProcess>){}
    virtual void visit(std::shared_ptr<const EventPropagationProcess>){}
    virtual void visit(std::shared_ptr<const RNGInitProcess>){};
    virtual void visit(std::shared_ptr<const Variable>){}
};

//----------------------------------------------------------------------------
// ModelComponent
//----------------------------------------------------------------------------
//! Base class for things that can be added to models
class ModelComponent : public std::enable_shared_from_this<ModelComponent>
{
public:
    ModelComponent(const ModelComponent&) = delete;
    virtual ~ModelComponent() = default;

    // Declared virtuals
    virtual void accept(ModelComponentVisitor &visitor) const = 0;

    // Public API
    const std::string &getName() const{ return m_Name; }

protected:
    struct Private{ explicit Private() = default; };

    ModelComponent(const std::string &name) : m_Name(name)
    {}

private:
    std::string m_Name;
};

//----------------------------------------------------------------------------
// Process
//----------------------------------------------------------------------------
class Process : public ModelComponent
{
protected:
    using ModelComponent::ModelComponent;
};

//----------------------------------------------------------------------------
// State
//----------------------------------------------------------------------------
class State : public ModelComponent
{
protected:
    using ModelComponent::ModelComponent;
};


//---------------------------------------------------------------------------
// AcceptableModelComponent
//---------------------------------------------------------------------------
template<typename T, typename B = ModelComponent>
class AcceptableModelComponent : public B
{
public:
    virtual void accept(ModelComponentVisitor &visitor) const final
    {
        visitor.visit(std::static_pointer_cast<const T>(this->shared_from_this()));
    }

protected:
    using B::B;
};