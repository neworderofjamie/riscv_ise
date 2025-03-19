#pragma once

// Standard C++ includes
#include <memory>
#include <vector>

// Compiler includes
#include "compiler/model_component.h"

// Forward declarations
class Process;

//----------------------------------------------------------------------------
// ProcessGroup
//----------------------------------------------------------------------------
class ProcessGroup : public AcceptableModelComponent<ProcessGroup>
{
public:
    ProcessGroup(Private, const std::vector<std::shared_ptr<const Process>> &processes, const std::string &name)
    :   AcceptableModelComponent<ProcessGroup>(name), m_Processes(processes)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getProcesses() const{ return m_Processes; }

    //------------------------------------------------------------------------
    // Static API
    //------------------------------------------------------------------------
    static std::shared_ptr<ProcessGroup> create(const std::vector<std::shared_ptr<const Process>> &processes, const std::string &name = "")
    {
        return std::make_shared<ProcessGroup>(Private(), processes, name);
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<std::shared_ptr<const Process>> m_Processes;
};