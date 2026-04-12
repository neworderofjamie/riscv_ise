#include "frontend/events.h"

// Common include
#include "common/utils.h"

//----------------------------------------------------------------------------
// Frontend::EventContainer
//----------------------------------------------------------------------------
namespace Frontend
{
void EventContainer::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    UPDATE_HASH_CLASS_NAME(EventContainer);
}
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> EventContainer::createArray(const Shape&, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing EventContainer::createArray implementation");
}

//----------------------------------------------------------------------------
// Frontend::EventChannel
//----------------------------------------------------------------------------
void EventChannel::updateMergeHash(boost::uuids::detail::sha1 &hash, const Model&) const
{
    UPDATE_HASH_CLASS_NAME(EventContainer);
}
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> EventChannel::createArray(const Shape&, const Model&, DeviceBase&) const
{
    throw std::runtime_error("Backend missing EventChannel::createArray implementation");
}
}