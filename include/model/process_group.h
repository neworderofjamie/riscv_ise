#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Compiler includes
#include "model/model_component.h"
#include "model/performance_counter.h"

// Forward declarations
namespace Model
{
class Process;
}

//----------------------------------------------------------------------------
// Model::ProcessGroup
//----------------------------------------------------------------------------
namespace Model
{
class ProcessGroup : public Stateful
{
public:
    ProcessGroup(Private, const std::vector<std::shared_ptr<const Process>> &processes, 
                 std::shared_ptr<const PerformanceCounter> performanceCounter, 
                 const std::string &name)
    :   Stateful(name), m_Processes(processes), m_PerformanceCounter(performanceCounter)
    {
    }

    //------------------------------------------------------------------------
    // Stateful virtuals
    //------------------------------------------------------------------------
    virtual std::vector<std::shared_ptr<const State>> getAllState() const override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getProcesses() const{ return m_Processes; }
    auto getPerformanceCounter() const{ return m_PerformanceCounter; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<ProcessGroup> create(const std::vector<std::shared_ptr<const Process>> &processes,
                                                std::shared_ptr<const PerformanceCounter> performanceCounter = nullptr,
                                                const std::string &name = "")
    {
        return std::make_shared<ProcessGroup>(Private(), processes, performanceCounter, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<std::shared_ptr<const Process>> m_Processes;
    std::shared_ptr<const PerformanceCounter> m_PerformanceCounter;
};
}