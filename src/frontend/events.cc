#include "frontend/events.h"

// Common include
#include "common/utils.h"

//----------------------------------------------------------------------------
// Frontend::EventSourceBuffer
//----------------------------------------------------------------------------
namespace Frontend
{
void EventSourceBuffer::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    UPDATE_HASH_CLASS_NAME(EventSourceBuffer);
}
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> EventSourceBuffer::createArray(std::optional<size_t>, uint32_t, 
                                                          size_t, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing EventSourceBuffer::createArray implementation");
}
//----------------------------------------------------------------------------
State::ShapeStride EventSourceBuffer::getArrayShapeStride(std::optional<size_t>, uint32_t, 
                                                          size_t, size_t, const Model&) const
{
    throw std::runtime_error("Backend missing EventSourceBuffer::getArrayShapeStride implementation");
}

//----------------------------------------------------------------------------
// Frontend::EventSinkBuffer
//----------------------------------------------------------------------------
void EventSinkBuffer::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    UPDATE_HASH_CLASS_NAME(EventSinkBuffer);
}
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> EventSinkBuffer::createArray(std::optional<size_t>, uint32_t, 
                                                        size_t, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing EventSinkBuffer::createArray implementation");
}
//----------------------------------------------------------------------------
State::ShapeStride EventSinkBuffer::getArrayShapeStride(std::optional<size_t>, uint32_t, 
                                                        size_t, size_t, const Model&) const
{
    throw std::runtime_error("Backend missing EventSinkBuffer::getArrayShapeStride implementation");
}

//----------------------------------------------------------------------------
// Frontend::EventChannelSource
//----------------------------------------------------------------------------
void EventChannelSource::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    UPDATE_HASH_CLASS_NAME(EventChannelSource);
}
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> EventChannelSource::createArray(std::optional<size_t>, uint32_t, 
                                                           size_t, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing EventChannelSource::createArray implementation");
}
//----------------------------------------------------------------------------
State::ShapeStride EventChannelSource::getArrayShapeStride(std::optional<size_t>, uint32_t, 
                                                           size_t, size_t, const Model&) const
{
    throw std::runtime_error("Backend missing EventChannelSource::getArrayShapeStride implementation");
}

//----------------------------------------------------------------------------
// Frontend::EventChannelSink
//----------------------------------------------------------------------------
void EventChannelSink::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    UPDATE_HASH_CLASS_NAME(EventChannelSink);
}
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> EventChannelSink::createArray(std::optional<size_t>, uint32_t, 
                                                         size_t, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing EventChannelSink::createArray implementation");
}
//----------------------------------------------------------------------------
State::ShapeStride EventChannelSink::getArrayShapeStride(std::optional<size_t>, uint32_t, 
                                                         size_t, size_t, const Model&) const
{
    throw std::runtime_error("Backend missing EventChannelSink::getArrayShapeStride implementation");
}
}