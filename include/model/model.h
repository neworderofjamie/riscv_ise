#pragma once

// Standard C++ includes
#include <memory>
#include <unordered_map>
#include <vector>

// Forward declarations
namespace Model
{
class Graph;
class State;
class Process;
}

//----------------------------------------------------------------------------
// Model::Model
//----------------------------------------------------------------------------
namespace Model
{
class Model
{
public:
    // Mapping of state objects to processes which reference them
    using StateProcesses = std::unordered_map<std::shared_ptr<const State>, std::vector<std::shared_ptr<const Process>>>;
    using GraphVector = std::vector<std::shared_ptr<const Graph>>;

    Model(const GraphVector &graphs);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getGraphs() const{ return m_Graphs; }
    const auto &getStateProcesses() const{ return m_StateProcesses; }

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    GraphVector m_Graphs;
    StateProcesses m_StateProcesses;
};
}