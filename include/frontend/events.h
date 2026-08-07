#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Frontend includes
#include "frontend/frontend_export.h"
#include "frontend/model_component.h"

//----------------------------------------------------------------------------
// Frontend::EventSink
//----------------------------------------------------------------------------
namespace Frontend
{
//! Event sinks take events from model components 
//! which emit them e.g. NeuronUpdateProcess
//! **NOTE** inherits State virtually to avoid diamond problem when EventSink AND EventSource are inherited
class FRONTEND_EXPORT EventSink : virtual public State
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
class FRONTEND_EXPORT EventSource : virtual public State
{
protected:
    using State::State;
};

//----------------------------------------------------------------------------
// Frontend::EventSourceBuffer
//----------------------------------------------------------------------------
//! A buffer which provides a source of events
class FRONTEND_EXPORT EventSourceBuffer : public EventSource
{
public:
    EventSourceBuffer(Private, const std::vector<size_t> &shape, size_t maxEvents, const std::string &name)
    :   State(name), EventSource(name), m_Shape(shape), m_MaxEvents(maxEvents)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual const std::vector<size_t> &getShape() const override final{ return m_Shape; }
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash) const override;

    virtual std::unique_ptr<ArrayBase> createArray(const std::vector<size_t> &shape, const std::vector<size_t> &stride,
                                                   const Model &model, DeviceBase &device) const override;

 
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    size_t getMaxEvents() const{ return m_MaxEvents; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventSourceBuffer> create(const std::vector<size_t> &shape, size_t maxEvents, const std::string &name = "")
    {
        return std::make_shared<EventSourceBuffer>(Private(), shape, maxEvents, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<size_t> m_Shape;
    size_t m_MaxEvents;
};

//----------------------------------------------------------------------------
// Frontend::EventSinkBuffer
//----------------------------------------------------------------------------
//! A buffer for recording events
class FRONTEND_EXPORT EventSinkBuffer : public EventSink
{
public:
    EventSinkBuffer(Private, const std::vector<size_t> &shape, const std::string &name)
    :   State(name), EventSink(name), m_Shape(shape)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual const std::vector<size_t> &getShape() const override final{ return m_Shape; }
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash) const override;

    virtual std::unique_ptr<ArrayBase> createArray(const std::vector<size_t> &shape, const std::vector<size_t> &stride,
                                                   const Model &model, DeviceBase &device) const override;


    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventSinkBuffer> create(const std::vector<size_t> &shape, const std::string &name = "")
    {
        return std::make_shared<EventSinkBuffer>(Private(), shape, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<size_t> m_Shape;
};

//----------------------------------------------------------------------------
// Frontend::EventChannel
//----------------------------------------------------------------------------
class FRONTEND_EXPORT EventChannel : public EventSource, public EventSink
{
public:
    EventChannel(Private, const std::vector<size_t> &shape, bool record, const std::string &name)
    :   State(name), EventSource(name), EventSink(name), m_Shape(shape), m_Record(record)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual const std::vector<size_t> &getShape() const override final{ return m_Shape; }

    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash) const override;

    virtual std::unique_ptr<ArrayBase> createArray(const std::vector<size_t> &shape, const std::vector<size_t> &stride,
                                                   const Model &model, DeviceBase &device) const override;
    
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    bool shouldRecord() const{ return m_Record; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<EventChannel> create(const std::vector<size_t> &shape, bool record = false, 
                                                const std::string &name = "")
    {
        return std::make_shared<EventChannel>(Private(), shape, record, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<size_t> m_Shape;
    bool m_Record;
};
}