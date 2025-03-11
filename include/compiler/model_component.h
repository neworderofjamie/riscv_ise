#pragma once

// Forward declarations
class EventContainer;
class Parameter;
class SpikeInputProcess;
class NeuronUpdateProcess;
class EventPropagationProcess;
class Variable;

//----------------------------------------------------------------------------
// ModelComponent
//----------------------------------------------------------------------------
class ModelComponentVisitor
{
public:
    virtual void visit(const EventContainer &eventContainer) = 0;
    virtual void visit(const Parameter &parameter) = 0;
    virtual void visit(const SpikeInputProcess &spikeInputProcess) = 0;
    virtual void visit(const NeuronUpdateProcess &neuronUpdateProcess) = 0;
    virtual void visit(const EventPropagationProcess &eventPropagationProcess) = 0;
    virtual void visit(const Variable &variable) = 0;
};

//----------------------------------------------------------------------------
// ModelComponent
//----------------------------------------------------------------------------
//! Base class for things that can be added to models
class ModelComponent
{
public:
    ModelComponent(const ModelComponent&) = delete;
    virtual ~ModelComponent() = default;

    // Declared virtuals
    virtual void accept(ModelComponentVisitor &visitor) const = 0;

protected:
    ModelComponent() = default;
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
        visitor.visit(static_cast<const T&>(*this));
    }
};