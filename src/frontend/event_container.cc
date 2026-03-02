#include "frontend/event_container.h"

// GeNN includes
#include "gennUtils.h"

using namespace GeNN;

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