#include "frontend/events.h"

// Common include
#include "common/utils.h"

//----------------------------------------------------------------------------
// Frontend::EventContainer
//----------------------------------------------------------------------------
namespace Frontend
{
void EventContainer::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    UPDATE_HASH_CLASS_NAME(EventContainer);
}

//----------------------------------------------------------------------------
// Frontend::EventChannel
//----------------------------------------------------------------------------
void EventChannel::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    UPDATE_HASH_CLASS_NAME(EventContainer);
}
}