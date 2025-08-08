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
class BackendFeNN;
class Process;
class ProcessGroup;
class State;
class Stateful;

//----------------------------------------------------------------------------
// Model
//----------------------------------------------------------------------------
//! Backend-agnostic datastructure build from processes user wishes to deploy
class BACKEND_EXPORT Model
{
public:
    // Mapping of state objects to field offset
    using StateFields = std::unordered_map<std::shared_ptr<const State>, uint32_t>;

    // Mapping of stateful model objects to their fields
    using StatefulFields = std::unordered_map<std::shared_ptr<const Stateful>, StateFields>;

    // Mapping of named global state to fields
    using BackendFields = std::unordered_map<std::string, uint32_t>;

    // Mapping of state objects to processes which reference them
    using StateProcesses = std::unordered_map<std::shared_ptr<const State>, std::vector<std::shared_ptr<const Process>>>;

    Model(const std::vector<std::shared_ptr<const ProcessGroup>> &processGroups, 
          const BackendFeNN &backend);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getProcessGroups() const{ return m_ProcessGroups; }
    const auto &getStatefulFields() const{ return m_StatefulFields; }
    const auto &getStateProcesses() const{ return m_StateProcesses; }
    const auto &getBackendFields() const {return m_BackendFields; }

    uint32_t getNumFields() const { return m_NumFields; }

private:
    std::vector<std::shared_ptr<const ProcessGroup>> m_ProcessGroups;
    StatefulFields m_StatefulFields;
    StateProcesses m_StateProcesses;
    BackendFields m_BackendFields;

    uint32_t m_NumFields;
};