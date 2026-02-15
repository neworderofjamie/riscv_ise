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
//! A model encompasses of a number of graphs which share the same state.
//! It also contains any additional data structures required BEFORE merging.
//! If a backend requires additional data to calculate mergibility, 
//! it needs to be added to a class derived from Model
namespace Model
{
class Model
{
public:
    // Mapping of state objects to processes which reference them
    using StateProcesses = std::unordered_map<std::shared_ptr<const State>, std::vector<std::shared_ptr<const Process>>>;
    using GraphVector = std::vector<std::shared_ptr<const Graph>>;

    Model(const GraphVector &graphs);
    virtual ~Model() = default;

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