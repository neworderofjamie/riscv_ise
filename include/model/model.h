#pragma once

// Standard C++ includes
#include <memory>
#include <unordered_map>
#include <vector>

// Forward declarations
namespace Model
{
class Kernel;
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
    using KernelVector = std::vector<std::shared_ptr<const Kernel>>;

    Model(const KernelVector &kernels);
    virtual ~Model() = default;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getKernels() const{ return m_Kernels; }
    const auto &getStateProcesses() const{ return m_StateProcesses; }

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    KernelVector m_Kernels;
    StateProcesses m_StateProcesses;
};
}