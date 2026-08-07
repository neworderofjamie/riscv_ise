#pragma once

// Standard C++ includes
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

// Frontend includes
#include "frontend/frontend_export.h"

// Forward declarations
namespace Frontend
{
class Kernel;
class Model;
class State;
class Process;
class ProcessGroup;
}

//----------------------------------------------------------------------------
// Frontend::MergedProcess
//----------------------------------------------------------------------------
namespace Frontend
{
class FRONTEND_EXPORT Padding
{
public:
    explicit Padding(size_t numAxes)
    :   m_PadMultiples(numAxes, 1)
    {}

    void update(size_t axis, std::optional<size_t> padMultiple);
    const auto &getPadMultiples() const{ return m_PadMultiples; }

private:
    std::vector<std::optional<size_t>> m_PadMultiples;
};

//----------------------------------------------------------------------------
// Frontend::Model
//----------------------------------------------------------------------------
//! A model encompasses of a number of graphs which share the same state.
//! It also contains any additional data structures required BEFORE merging.
//! If a backend requires additional data to calculate mergibility, 
//! it needs to be added to a class derived from Model
class FRONTEND_EXPORT Model
{
public:
    struct StateData
    {
        std::optional<size_t> splitDimension;
        std::vector<std::optional<size_t>> padMultiples;

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
    const StateData &getStateData(std::shared_ptr<const Frontend::State> state) const;

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    KernelVector m_Kernels;
    std::unordered_map<std::shared_ptr<const State>, StateData> m_StateData;
};
}