#pragma once

// Standard C++ includes
#include <algorithm>
#include <iterator>
#include <memory>
#include <unordered_map>
#include <vector>

// Forward declarations
namespace Model
{
class Model;
class Process;
class ProcessGroup;
class State;
}

//----------------------------------------------------------------------------
// Backend::MergedProcess
//----------------------------------------------------------------------------
namespace Backend
{
class MergedProcess
{
public:
    MergedProcess(size_t index, const std::vector<std::shared_ptr<::Model::Process const>> &processes)
    :   m_Index(index), m_Processes(processes)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    size_t getIndex() const { return m_Index; }

    //! Get 'archetype' process - it's properties represent those of all other merged processes
    const auto &getArchetype() const { return m_Processes.front(); }

    //! Get 'archetype' process - it's properties represent those of all other merged processes
    template<typename P>
    const auto &getArchetype() const 
    { 
        return std::dynamic_pointer_cast<const P>(getArchetype());
    }

    //! Get vector of merged processes
    const auto &getProcesses() const{ return m_Processes; }

    //! 'Gathers' a vector of shared pointers to state obtained by calling a function on each merged process
    template<typename P, typename F>
    auto gatherState(F getStateFn) const
    {
        // Reserve vector to hold state gathered from all processes
        std::vector<std::shared_ptr<const ::Model::State>> state;
        state.reserve(getProcesses().size());

        std::transform(getProcesses().cbegin(), getProcesses().cend(), std::back_inserter(state),
                       [getStateFn](const auto &p)
                       {
                           // Cast to desired type
                           auto derivedP = std::dynamic_pointer_cast<const P>(p);
                           assert(derivedP);

                           return getStateFn(derivedP);
                       });
        return state;
    }

    template<typename P, typename F>
    void forEachProcess(F func) const
    {
        // Loop through processes
        for (const auto &p : getProcesses()) {
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
    std::vector<std::shared_ptr<::Model::Process const>> m_Processes;
};

//----------------------------------------------------------------------------
// Backend::MergedModel
//----------------------------------------------------------------------------
class MergedModel
{
public:
    MergedModel(const ::Model::Model &model);

    //----------------------------------------------------------------------------
    // Public API
    //----------------------------------------------------------------------------
    const auto &getModel() const{ return m_Model.get(); }

    //! Get map of process groups to merged processes
    const auto &getMergedProcessGroups() const{ return m_MergedProcessGroups; }

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    std::reference_wrapper<const ::Model::Model> m_Model;
    
    // Map of process groups to vector of mergeable processes
    std::unordered_map<std::shared_ptr<::Model::ProcessGroup const>, 
                       std::vector<MergedProcess>> m_MergedProcessGroups;
};
}