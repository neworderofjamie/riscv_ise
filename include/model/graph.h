#pragma once

#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Compiler includes
#include "model/model_component.h"

// Forward declarations
namespace Model
{
class ProcessGroup;
}


namespace Model
{
using ProcessGroupVector = std::vector<std::shared_ptr<const ProcessGroup>>;

//----------------------------------------------------------------------------
// Model::Graph
//----------------------------------------------------------------------------
class Graph : public Stateful
{
public:
    using Stateful::Stateful;

    //----------------------------------------------------------------------------
    // Declared virtuals
    //----------------------------------------------------------------------------
    //! Get all process groups in graph
    virtual ProcessGroupVector getAllProcessGroups() const = 0;
};

//----------------------------------------------------------------------------
// Model::StandardGraph
//----------------------------------------------------------------------------
class SimpleGraph : public Graph
{
public:
    SimpleGraph(Private, const ProcessGroupVector &processGroups, const std::string &name)
    :   Graph(name), m_ProcessGroups(processGroups)
    {}

    //----------------------------------------------------------------------------
    // Declared virtuals
    //----------------------------------------------------------------------------
    //! Get all process groups in graph
    virtual ProcessGroupVector getAllProcessGroups() const override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getProcessGroups() const{ return m_ProcessGroups; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    ProcessGroupVector m_ProcessGroups;
};

//----------------------------------------------------------------------------
// Model::SimulationLoopGraph
//----------------------------------------------------------------------------
class SimulationLoopGraph : public Graph
{
public:
    SimulationLoopGraph(Private, unsigned int numTimesteps, 
                        const ProcessGroupVector &timestepProcessGroups, 
                        const ProcessGroupVector &beginProcessGroups,
                        const ProcessGroupVector &endProcessGroups, const std::string &name)
    :   Graph(name), m_NumTimesteps(numTimesteps), m_TimestepProcessGroups(timestepProcessGroups),
        m_BeginProcessGroups(beginProcessGroups), m_EndProcessGroups(endProcessGroups)
    {}

    //----------------------------------------------------------------------------
    // Declared virtuals
    //----------------------------------------------------------------------------
    //! Get all process groups in graph
    virtual ProcessGroupVector getAllProcessGroups() const override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    auto getNumTimesteps() const{ return m_NumTimesteps; }
    const auto &getTimestepProcessGroups() const{ return m_TimestepProcessGroups; }
    const auto &getBeginProcessGroups() const{ return m_BeginProcessGroups; }
    const auto &getEndProcessGroups() const{ return m_EndProcessGroups; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    unsigned int m_NumTimesteps;
    ProcessGroupVector m_TimestepProcessGroups;
    ProcessGroupVector m_BeginProcessGroups;
    ProcessGroupVector m_EndProcessGroups;
};
}
