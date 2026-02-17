#include "backend/runtime.h"

// Model includes
#include "model/model.h"
#include "model/model_component.h"

//--------------------------------------------------------------------------
// Backend::ArrayBase
//--------------------------------------------------------------------------
namespace Backend
{
void ArrayBase::memsetHostPointer(int value)
{
    std::memset(m_HostPointer, value, getSizeBytes());
}

//----------------------------------------------------------------------------
// Runtime
//----------------------------------------------------------------------------
Runtime::Runtime(const ::Model::Model &model)
:   m_MergedModel(model)
{
    // 1) Generate code - fields must be allocated here
    // 2) Build code
}
//----------------------------------------------------------------------------
void Runtime::allocate()
{
    // Create special array to hold field information
    // **THINK** fields need to have been allocated at this point
    //m_FieldArray = m_State->createFieldArray(m_Model.get());
    // Perform backend-specific logic
    allocatePreamble();

    // Loop through state objects used by model
    for (const auto &s : m_MergedModel.getModel().getStateProcesses()) {
        // Create array
        auto array = createArray(s.first);

        // Take ownership of array and add to arrays map
        if (!m_Arrays.try_emplace(s.first, std::move(array)).second) {
            throw std::runtime_error("Duplicate array found for state '" + s.first->getName() + "'");
        }
    }

    // Loop through all process groups
    for(const auto &g : m_MergedModel.getMergedProcessGroups()) {
        // Loop through all merged processes and setup fields
        for (const auto &m : g.second) {
            setMergedProcessFields(m);
        }
    }

    // Perform backend-specific logic
    allocatePostamble();
    // Loop through all stateful objects with fields that require population
    /*for(const auto &p : m_Model.get().getStatefulFields()) {
        // Loop through all fields associated with stateful object and assign corresponding arrays to field
        for(const auto &s : p.second) {
            m_FieldArray->setFieldArray(s.second, getArray(s.first));
        }
    }*/

    // Loop through all fields requested by the backend 
    /*for(const auto &b : m_Model.get().getBackendFields()) {
        // Create array
        auto fieldArray = m_Backend.get().createArray(std::get<0>(b.second), b.first, m_State.get());
        
        // Assign array to field
        m_FieldArray->setFieldArray(std::get<1>(b.second), fieldArray.get());

        // Take ownership of array and add to arrays map
        if (!m_Arrays.try_emplace(std::get<0>(b.second), std::move(fieldArray)).second) {
            assert(false);
        }
    }*/

    // Push populated fields to device
    //m_FieldArray->pushFieldsToDevice();
}
//----------------------------------------------------------------------------
ArrayBase *Runtime::getArray(std::shared_ptr<const ::Model::State> variable) const
{
    return m_Arrays.at(variable).get();
}
//----------------------------------------------------------------------------
/*std::optional<unsigned int> Runtime::getSOCPower() const
{
    return m_State->getSOCPower();
}*/
}