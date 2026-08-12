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
                                                          size_t, const Model&, const DeviceBase&) const
{
    throw std::runtime_error("Backend missing Variable::EventSourceBuffer implementation");
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
                                                        size_t, const Model&, const DeviceBase&) const
{
    throw std::runtime_error("Backend missing Variable::EventSinkBuffer implementation");
}

//----------------------------------------------------------------------------
// Frontend::EventChannel
//----------------------------------------------------------------------------
void EventChannel::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    UPDATE_HASH_CLASS_NAME(EventChannel);
}
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> EventChannel::createArray(std::optional<size_t>, uint32_t, 
                                                     size_t, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing EventChannel::createArray implementation");
}
//----------------------------------------------------------------------------
State::ShapeStride EventChannel::getArrayShapeStride(std::optional<size_t>, uint32_t, 
                                                     size_t, const Model&, const DeviceBase&) const
{
    throw std::runtime_error("Backend missing Variable::EventChannel implementation");
}
}