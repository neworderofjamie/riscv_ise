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
// CodeGenerator::State
//----------------------------------------------------------------------------
namespace CodeGenerator
{
class State
{
public:
    // Mapping of state objects to processes which reference them
    using StateProcesses = std::unordered_map<std::shared_ptr<const Model::State>, std::vector<std::shared_ptr<const Model::Process>>>;
    using GraphVector = std::vector<std::shared_ptr<const Model::Graph>>;

    State(const GraphVector &graphs);

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