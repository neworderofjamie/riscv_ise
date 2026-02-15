#include "model/graph.h"

// Standard C++ includes
#include <iterator>

//----------------------------------------------------------------------------
// Model::SimpleGraph
//----------------------------------------------------------------------------
namespace Model
{
ProcessGroupVector SimpleGraph::getAllProcessGroups() const
{
    return this->getProcessGroups();
}

//----------------------------------------------------------------------------
// Model::SimulationLoopGraph
//----------------------------------------------------------------------------
ProcessGroupVector SimulationLoopGraph::getAllProcessGroups() const
{
    // Reserve vector
    ProcessGroupVector processGroups;
    processGroups.reserve(getBeginProcessGroups().size()
                          + getEndProcessGroups().size()
                          + getTimestepProcessGroups().size());

    // Copy in all process groups and return
    std::copy(getBeginProcessGroups().cbegin(), getBeginProcessGroups().cend(), 
              std::back_inserter(processGroups));
    std::copy(getEndProcessGroups().cbegin(), getEndProcessGroups().cend(), 
              std::back_inserter(processGroups));
    std::copy(getTimestepProcessGroups().cbegin(), getTimestepProcessGroups().cend(), 
              std::back_inserter(processGroups));
    return processGroups;
}
}