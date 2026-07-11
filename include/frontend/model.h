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
template<typename T>
class Merged
{
public:
    Merged(size_t index, const std::vector<std::shared_ptr<const T>> &merged)
        :   m_Index(index), m_Merged(merged)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    size_t getIndex() const { return m_Index; }

    //! Get 'archetype' process - it's properties represent those of all other merged processes
    const auto &getArchetype() const { return m_Merged.front(); }

    //! Get 'archetype' process - it's properties represent those of all other merged processes
    template<typename P>
    const auto getArchetype() const 
    { 
        return std::dynamic_pointer_cast<const P>(getArchetype());
    }

    //! Get vector of merged processes
    const auto &getMerged() const{ return m_Merged; }

    template<typename P = Process, typename F>
    void forEachMerged(F func) const
    {
        // Loop through processes
        for (const auto &p : getMerged()) {
            // Cast to desired type
            auto derivedP = std::dynamic_pointer_cast<const P>(p);
            assert(derivedP);

            // Call function
            func(derivedP);
        }
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    size_t m_Index;
    std::vector<std::shared_ptr<T const>> m_Merged;
};

using MergedProcess = Merged<Process>;

//----------------------------------------------------------------------------
// Frontend::MergedProcessGroup
//----------------------------------------------------------------------------
class MergedProcessGroup
{
public:
    //! The archetype process it has been merged with and
    using Destination = std::pair<std::shared_ptr<Process const>, size_t>;

    MergedProcessGroup(const Model &model, std::shared_ptr<ProcessGroup const> processGroup);

    //----------------------------------------------------------------------------
    // Public API
    //----------------------------------------------------------------------------
    //! Get vector of merged processes within this groups
    const auto &getMergedProcesses() const{ return m_MergedProcesses; }

    //! Determine which merged process a process has ended up in following merging
    const Destination &getDestination(std::shared_ptr<Process const> process) const
    {
        return m_Destinations.at(process);
    }

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    std::vector<MergedProcess> m_MergedProcesses;

    // Reverse look up structure to find where process ended up in merging process
    std::unordered_map<std::shared_ptr<Process const>, Destination> m_Destinations;
};

//----------------------------------------------------------------------------
// Frontend::Model
//----------------------------------------------------------------------------
//! A model encompasses of a number of graphs which share the same state.
//! It also contains any additional data structures required BEFORE merging.
//! If a backend requires additional data to calculate mergibility, 
//! it needs to be added to a class derived from Model
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
    const StateData &getStateData(std::shared_ptr<const Frontend::State> state) const;

    //! Get map of process groups to merged processes
    const auto &getMergedProcessGroups() const{ return m_MergedProcessGroups; }

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    KernelVector m_Kernels;
    std::unordered_map<std::shared_ptr<const State>, StateData> m_StateData;

    // Map of process groups to merged process groups
    std::unordered_map<std::shared_ptr<ProcessGroup const>, MergedProcessGroup> m_MergedProcessGroups;
};
}