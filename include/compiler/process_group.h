#pragma once

// Standard C++ includes
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
    ProcessGroup(const std::vector<const Process*> processes, const std::string &name = "")
    :   AcceptableModelComponent<ProcessGroup>(name), m_Processes(processes)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getProcesses() const{ return m_Processes; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<const Process*> m_Processes;
};