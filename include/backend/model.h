#pragma once

// Standard C++ includes
#include <memory>
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cstdint>

// Backend includes
#include "backend/backend_export.h"

// Forward declarations
class Process;
class ProcessGroup;
class State;

//----------------------------------------------------------------------------
// Model
//----------------------------------------------------------------------------
//! Backend-agnostic datastructure build from processes user wishes to deploy
class BACKEND_EXPORT Model
{
public:
    // Mapping of state objects to field offset
    using StateFields = std::unordered_map<std::shared_ptr<const State>, uint32_t>;

    // Mapping of processes to their fields
    using ProcessFields = std::unordered_map<std::shared_ptr<const Process>, StateFields>;

    // Mapping of process groups to their fields
    using ProcessGroupFields = std::unordered_map<std::shared_ptr<const ProcessGroup>, StateFields>;

    // Mapping of state objects to processes which reference them
    using StateProcesses = std::unordered_map<std::shared_ptr<const State>, std::vector<std::shared_ptr<const Process>>>;

    Model(const std::vector<std::shared_ptr<const ProcessGroup>> &processGroups);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getProcessGroups() const{ return m_ProcessGroups; }
    const auto &getProcessFields() const{ return m_ProcessFields; }
    const auto &getProcessGroupFields() const{ return m_ProcessGroupFields; }
    const auto &getStateProcesses() const{ return m_StateProcesses; }

    uint32_t getNumFields() const { return m_NumFields; }

private:
    std::vector<std::shared_ptr<const ProcessGroup>> m_ProcessGroups;
    ProcessFields m_ProcessFields;
    ProcessGroupFields m_ProcessGroupFields;
    StateProcesses m_StateProcesses;
    uint32_t m_NumFields;
};