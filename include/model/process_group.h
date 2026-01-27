#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Compiler includes
#include "model/model_component.h"
#include "model/performance_counter.h"

// Forward declarations
class Process;

//----------------------------------------------------------------------------
// ProcessGroup
//----------------------------------------------------------------------------
class ProcessGroup : public AcceptableModelComponent<ProcessGroup, Stateful>
{
public:
    ProcessGroup(Private, const std::vector<std::shared_ptr<const Process>> &processes, 
                 std::shared_ptr<const PerformanceCounter> performanceCounter, 
                 const std::string &name)
    :   AcceptableModelComponent<ProcessGroup, Stateful>(name), m_Processes(processes),
        m_PerformanceCounter(performanceCounter)
    {
    }

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