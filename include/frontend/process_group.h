#pragma once

// Standard C++ includes
#include <memory>
#include <set>
#include <vector>

// Compiler includes
#include "frontend/frontend_export.h"
#include "frontend/model_component.h"

// Forward declarations
namespace Frontend
{
class Process;
}

//----------------------------------------------------------------------------
// Frontend::ProcessGroup
//----------------------------------------------------------------------------
namespace Frontend
{
class FRONTEND_EXPORT ProcessGroup : public ModelComponent
{
public:
    ProcessGroup(Private, const std::vector<std::shared_ptr<const Process>> &processes, 
                 const std::vector<std::string> &performanceCounters, const std::string &name)
    :   ModelComponent(name), m_Processes(processes), 
        m_PerformanceCounters(performanceCounters.cbegin(), performanceCounters.cend())
    {
    }

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getProcesses() const{ return m_Processes; }
    const auto &getPerformanceCounters() const{ return m_PerformanceCounters; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<ProcessGroup> create(const std::vector<std::shared_ptr<const Process>> &processes,
                                                const std::vector<std::string> &performanceCounters = {}, const std::string &name = "")
    {
        return std::make_shared<ProcessGroup>(Private(), processes, performanceCounters, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<std::shared_ptr<const Process>> m_Processes;
    std::set<std::string> m_PerformanceCounters;
};
}
