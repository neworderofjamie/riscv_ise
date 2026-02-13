#include "model/event_container.h"

// GeNN includes
#include "gennUtils.h"

using namespace GeNN::Utils;

//----------------------------------------------------------------------------
// Model::EventContainer
//----------------------------------------------------------------------------
namespace Model
{
void EventContainer::updateMergeHash(boost::uuids::detail::sha1 &hash) const
{
    updateHash(getNumBufferTimesteps(), hash);
}
}