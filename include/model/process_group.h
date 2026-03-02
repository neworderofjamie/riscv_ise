#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Compiler includes
#include "model/model_component.h"

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
class ProcessGroup : public ModelComponent
{
public:
    ProcessGroup(Private, const std::vector<std::shared_ptr<const Process>> &processes, 
                 bool recordPerformance, const std::string &name)
    :   ModelComponent(name), m_Processes(processes), m_RecordPerformance(recordPerformance)
    {
    }

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getProcesses() const{ return m_Processes; }
    bool shouldRecordPerformance() const{ return m_RecordPerformance; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<ProcessGroup> create(const std::vector<std::shared_ptr<const Process>> &processes,
                                                bool recordPerformance = false, const std::string &name = "")
    {
        return std::make_shared<ProcessGroup>(Private(), processes, recordPerformance, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<std::shared_ptr<const Process>> m_Processes;
    bool m_RecordPerformance;
};
}
