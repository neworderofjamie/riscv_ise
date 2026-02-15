#include "model/process_group.h"

//----------------------------------------------------------------------------
// Model::ProcessGroup
//----------------------------------------------------------------------------
namespace Model
{
std::vector<std::shared_ptr<const State>> ProcessGroup::getAllState() const
{
    if(getPerformanceCounter()) {
        return {getPerformanceCounter()};
    }
    else {
        return {};
    }
}
}