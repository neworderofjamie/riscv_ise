#pragma once

// Standard C++ includes
#include <algorithm>
#include <memory>
#include <stdexcept>
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
class FRONTEND_EXPORT EventSink : public State
{
protected:
    using State::State;
};

//----------------------------------------------------------------------------
// Frontend::EventSource
//----------------------------------------------------------------------------
//! Event sources produce events for model components
//! which require them e.g. EventPropagationProcess
class FRONTEND_EXPORT EventSource : public State
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
    :   EventSource(name), m_Shape(shape), m_MaxEvents(maxEvents)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual const std::vector<size_t> &getShape() const override final{ return m_Shape; }
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash) const override;

    virtual std::unique_ptr<ArrayBase> createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                   size_t numDevices, const Model &model, DeviceBase &device) const override;

    virtual ShapeStride getArrayShapeStride(std::optional<size_t> splitDimension, uint32_t indexDimensions, 
                                            size_t deviceIndex, size_t numDevices, const Model &model) const override;

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
//! A buffer for recording events emitted from a process
class FRONTEND_EXPORT EventSinkBuffer : public EventSink
{
public:
    EventSinkBuffer(Private, const std::vector<size_t> &shape, const std::string &name)
    :   EventSink(name), m_Shape(shape)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual const std::vector<size_t> &getShape() const override final{ return m_Shape; }
    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash) const override;

    virtual std::unique_ptr<ArrayBase> createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                   size_t numDevices, const Model &model, DeviceBase &device) const override;

    virtual ShapeStride getArrayShapeStride(std::optional<size_t> splitDimension, uint32_t indexDimensions, 
                                            size_t deviceIndex, size_t numDevices, const Model &model) const override;

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
// Frontend::EventChannelSource
//----------------------------------------------------------------------------
//! Event source at the output end of an event channel
class FRONTEND_EXPORT EventChannelSource : public EventSource
{
public:
    EventChannelSource(Private, const std::vector<size_t> &shape, const std::string &name)
    :   EventSource(name), m_Shape(shape)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual const std::vector<size_t> &getShape() const override final{ return m_Shape; }

    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash) const override;

    virtual std::unique_ptr<ArrayBase> createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                   size_t numDevices, const Model &model, DeviceBase &device) const override;

    virtual ShapeStride getArrayShapeStride(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                            size_t deviceIndex, size_t numDevices, const Model &model) const override;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<size_t> m_Shape;
};

//----------------------------------------------------------------------------
// Frontend::EventChannelSink
//----------------------------------------------------------------------------
//! Event sink at the input end of an event channel
class FRONTEND_EXPORT EventChannelSink : public EventSink
{
public:
    EventChannelSink(Private, const std::vector<size_t> &shape, 
                     std::weak_ptr<const EventChannelSource> source,
                     bool record, const std::string &name)
    :   EventSink(name), m_Shape(shape), m_Source(source), m_Record(record)
    {}

    //------------------------------------------------------------------------
    // State virtuals
    //------------------------------------------------------------------------
    virtual const std::vector<size_t> &getShape() const override final{ return m_Shape; }

    virtual void updateMergeHash(boost::uuids::detail::sha1 &hash) const override;

    virtual std::unique_ptr<ArrayBase> createArray(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                                   size_t numDevices, const Model &model, DeviceBase &device) const override;

    virtual ShapeStride getArrayShapeStride(std::optional<size_t> splitDimension, uint32_t indexDimensions,
                                            size_t deviceIndex, size_t numDevices, const Model &model) const override;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    auto getSource() const{ return m_Source; }
    bool shouldRecord() const{ return m_Record; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<size_t> m_Shape;
    std::weak_ptr<const EventChannelSource> m_Source;
    bool m_Record;
};

//----------------------------------------------------------------------------
// Frontend::EventChannel
//----------------------------------------------------------------------------
class FRONTEND_EXPORT EventChannel : public ModelComponent
{
public:
    EventChannel(Private, std::shared_ptr<const EventChannelSink> sink,
                 std::shared_ptr<const EventChannelSource> source, const std::string &name)
    :   ModelComponent(name), m_Sink(sink), m_Source(source)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    auto getSink() const{ return m_Sink; }
    auto getSource() const{ return m_Source; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    template<typename Sink = EventChannelSink, typename Source = EventChannelSource>
    static std::shared_ptr<EventChannel> create(const std::vector<size_t> &sinkShape, 
                                                const std::vector<size_t> &sourceShape, 
                                                bool record = false, const std::string &name = "")
    {
        // If we are recording
        if (record) {
            // Sink shape should have one extra (time) dimension
            if (sinkShape.size() != (1 + sourceShape.size())) {
                throw std::runtime_error("When recording, event channel sink should "
                                         "specify number of timesteps to record in its shape");
            }

            // Check remaining dimensions match
            if (!std::equal(sinkShape.cbegin() + 1, sinkShape.cend(), sourceShape)) {
                throw std::runtime_error("Event channel sink and source shapes must match");
            }
        }
        // Otherwise, source and sink shapes must match
        else if (sinkShape != sourceShape) {
            throw std::runtime_error("Event channel sink and source shapes must match");
        }
        // Create source
        auto source = std::make_unique<const Source>(Private(), sourceShape, 
                                                     name.empty() ? "" : name + "_source");

        // Create sink
        auto sink = std::make_unique<const Sink>(Private(), sinkShape, source, record, 
                                                 name.empty() ? "" : name + "_sink");

        // Create channel
        return std::make_shared<EventChannel>(Private(), sink, source, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::shared_ptr<const EventChannelSink> m_Sink;
    std::shared_ptr<const EventChannelSource> m_Source;
};
}