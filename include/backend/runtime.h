#pragma once

// Standard C++ includes
#include <memory>
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cstdint>>

// Backend includes
#include "backend/process_fields.h"

// Forward declarations
class ArrayBase;
class BackendFeNN;
class State;
class StateBase;

//----------------------------------------------------------------------------
// Runtime
//----------------------------------------------------------------------------
class Runtime
{
public:
    Runtime(const ProcessFields &processFields, const BackendFeNN &backend);
    
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void setInstructions(const std::vector<uint32_t> &instructions);

    void allocate();
    void run();

    ArrayBase *getArray(std::shared_ptr<State> variable) const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unique_ptr<StateBase> m_State;
    std::unordered_map<std::shared_ptr<const State>, std::unique_ptr<ArrayBase>> m_Arrays;
    std::reference_wrapper<const ProcessFields> m_ProcessFields;
};