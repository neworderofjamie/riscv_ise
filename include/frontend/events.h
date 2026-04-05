#pragma once

// Standard C++ includes
#include <memory>

// Frontend includes
#include "frontend/frontend_export.h"
#include "frontend/model_component.h"
#include "frontend/shape.h"

//----------------------------------------------------------------------------
// Frontend::EventSink
//----------------------------------------------------------------------------
namespace Frontend
{
//! Event sinks take events from model components 
//! which emit them e.g. NeuronUpdateProcess
//! **NOTE** inherits State virtually to avoid diamond problem when EventSink AND EventSource are inherited
class EventSink : virtual public State
{
protected:
    using State::State;
};

//----------------------------------------------------------------------------
// Frontend::EventSource
//----------------------------------------------------------------------------
//! Event sources produce events for model components
//! which require them e.g. EventPropagationProcess
//! **NOTE** inherits State virtually to avoid diamond problem when EventSink AND EventSource are inherited
class EventSource : virtual public State
{
protected:
    using State::State;
};

//----------------------------------------------------------------------------
// Frontend::EventContainer
//----------------------------------------------------------------------------
class EventContainer : public EventSource
{
public:
    EventContainer(Private, const Shape &shape, const std::string &name)
    :   State(name), EventSource(name), m_Shape(shape)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual void accept(StateVisitor &visitor) const override final
    {
        visitor.visit(std::dynamic_pointer_cast<const EventContainer>(this->shared_from_this()));
    }

    virtual const Shape &getShape() const override final{ return m_Shape; }
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash) const override;

 
    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventContainer> create(const Shape &shape, const std::string &name = "")
    {
        return std::make_shared<EventContainer>(Private(), shape, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Shape m_Shape;
};

//----------------------------------------------------------------------------
// Frontend::EventChannel
//----------------------------------------------------------------------------
class EventChannel : public EventSource, public EventSink
{
public:
    EventChannel(Private, const Shape &shape, const std::string &name)
    :   State(name), EventSource(name), EventSink(name), m_Shape(shape)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual void accept(StateVisitor &visitor) const override final
    {
        visitor.visit(std::dynamic_pointer_cast<const EventChannel>(this->shared_from_this()));
    }

    virtual const Shape &getShape() const override final{ return m_Shape; }

    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash) const override;

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventChannel> create(const Shape &shape, const std::string &name = "")
    {
        return std::make_shared<EventChannel>(Private(), shape, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Shape m_Shape;
};
}