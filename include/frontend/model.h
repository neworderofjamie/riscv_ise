#pragma once

// Standard C++ includes
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

// Forward declarations
namespace Frontend
{
class Kernel;
class State;
class Process;
}

//----------------------------------------------------------------------------
// Frontend::Model
//----------------------------------------------------------------------------
//! A model encompasses of a number of graphs which share the same state.
//! It also contains any additional data structures required BEFORE merging.
//! If a backend requires additional data to calculate mergibility, 
//! it needs to be added to a class derived from Model
namespace Frontend
{
class Model
{
public:
    struct StateData
    {
        std::optional<size_t> splitDimension;
        std::vector<std::shared_ptr<const Process>> processes;
    };

    using KernelVector = std::vector<std::shared_ptr<const Kernel>>;

    Model(const KernelVector &kernels);
    virtual ~Model() = default;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getKernels() const{ return m_Kernels; }
    const auto &getStateData() const{ return m_StateData; }

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    KernelVector m_Kernels;
    std::unordered_map<std::shared_ptr<const State>, StateData> m_StateData;
};
}