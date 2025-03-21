#pragma once

// Standard C++ includes
#include <memory>
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cstdint>

// Forward declarations
class ArrayBase;
class BackendFeNN;
class Model;
class State;
class StateBase;

//----------------------------------------------------------------------------
// Runtime
//----------------------------------------------------------------------------
class Runtime
{
public:
    Runtime(const Model &model, const BackendFeNN &backend);
    
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void setInstructions(const std::vector<uint32_t> &instructions);

    void allocate();
    void run();

    ArrayBase *getArray(std::shared_ptr<const State> variable) const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unique_ptr<StateBase> m_State;
    std::unordered_map<std::shared_ptr<const State>, std::unique_ptr<ArrayBase>> m_Arrays;
    std::reference_wrapper<const Model> m_Model;
};