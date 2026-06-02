#include "frontend/events.h"

// Common include
#include "common/utils.h"

//----------------------------------------------------------------------------
// Frontend::EventSourceBuffer
//----------------------------------------------------------------------------
namespace Frontend
{
void EventSourceBuffer::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    UPDATE_HASH_CLASS_NAME(EventSourceBuffer);
}
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> EventSourceBuffer::createArray(const Shape&, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing EventSourceBuffer::createArray implementation");
}

//----------------------------------------------------------------------------
// Frontend::EventSinkBuffer
//----------------------------------------------------------------------------
void EventSinkBuffer::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    UPDATE_HASH_CLASS_NAME(EventSinkBuffer);
}
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> EventSinkBuffer::createArray(const Shape&, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing EventSinkBuffer::createArray implementation");
}

//----------------------------------------------------------------------------
// Frontend::EventChannel
//----------------------------------------------------------------------------
void EventChannel::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    UPDATE_HASH_CLASS_NAME(EventChannel);
}
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> EventChannel::createArray(const Shape&, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing EventChannel::createArray implementation");
}
}