#include "frontend/event_container.h"

// Common includes
#include "common/utils.h"

using namespace Common;

//----------------------------------------------------------------------------
// Frontend::EventContainer
//----------------------------------------------------------------------------
namespace Frontend
{
void EventContainer::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    Utils::updateHash(getNumBufferTimesteps(), hash);
}
}
