#pragma once

// Standard C++ includes
#include <memory>
#include <unordered_map>
#include <vector>

// Forward declarations
namespace Model
{
class Model;
class Process;
class ProcessGroup;
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
    :   m_Index(index), m_Processes(std::move(processes))
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    size_t getIndex() const { return m_Index; }

    //! Get 'archetype' process - it's properties represent those of all other merged processes
    const auto &getArchetype() const { return m_Processes.front(); }

    //! Get vector of merged processes
    const auto &getProcesses() const{ return m_Processes; }

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

    //! Get map of process groups to merged processes
    const auto &getMergedProcessGroups() const{ return m_MergedProcessGroups; }

private:
    //----------------------------------------------------------------------------
    // Members
    // //----------------------------------------------------------------------------
    // Map of process groups to vector of mergeable processes
    std::unordered_map<std::shared_ptr<::Model::ProcessGroup const>, 
                       std::vector<MergedProcess>> m_MergedProcessGroups;
};
}