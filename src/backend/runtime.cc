#include "backend/runtime.h"

// Backend includes
#include "backend/backend_fenn.h"
#include "backend/model.h"

//----------------------------------------------------------------------------
// Runtime
//----------------------------------------------------------------------------
Runtime::Runtime(const Model &model, const BackendFeNN &backend)
:   m_Model(model)
{
    m_State = backend.createState();
}
//----------------------------------------------------------------------------
void Runtime::setInstructions(const std::vector<uint32_t> &instructions)
{
    m_State->setInstructions(instructions);
}
//----------------------------------------------------------------------------
void Runtime::allocate()
{
    // **TODO** allocate BRAM array to hold process fields

    // Visit variables and event containers, ask backend to create suitable array for them
    // **THINK** how will this handle the fact that different variables 
    // get implemented as different sorts of array in different situations
    // this also needs to have been decided before this point as it effects code generation

    // Loop through all the processes with fields that require population
    for(const auto &p : m_Model.get().getProcessFields()) {
        // Loop through all fields associated with process
        for(const auto &s : p.second) {
            // Get array corresponding to state object this field is fore
            const auto *array = getArray(s.first);

            // **TODO** serialise device pointer of array into process fields array
        }
    }

    // **TODO** push process fields to device
}
//----------------------------------------------------------------------------
void Runtime::run()
{
    m_State->run();
}
//----------------------------------------------------------------------------
ArrayBase *Runtime::getArray(std::shared_ptr<const State> variable) const
{
    return m_Arrays.at(variable).get();
}
