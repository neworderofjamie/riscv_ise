#include "backend/runtime.h"

// Backend includes
#include "backend/backend_fenn.h"
#include "backend/model.h"

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
//----------------------------------------------------------------------------
// Visitor
//----------------------------------------------------------------------------
class AllocatorVisitor : public ModelComponentVisitor
{
public:
    AllocatorVisitor(std::shared_ptr<const State> modelState, const BackendFeNN &backend, StateBase *state)
    :   m_Backend(backend), m_State(state)
    {
        modelState->accept(*this);
    }

    auto &getArray() { return m_Array; }

private:
    //------------------------------------------------------------------------
    // ModelComponentVisitor virtuals
    //------------------------------------------------------------------------
    virtual void visit(std::shared_ptr<const EventContainer> eventContainer)
    {
        m_Array = m_Backend.get().createArray(eventContainer, m_State);
    }

    virtual void visit(std::shared_ptr<const Variable> variable)
    {
        m_Array = m_Backend.get().createArray(variable, m_State);
    }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::reference_wrapper<const BackendFeNN> m_Backend;
    std::unique_ptr<ArrayBase> m_Array;
    StateBase *m_State;
};
}

//----------------------------------------------------------------------------
// Runtime
//----------------------------------------------------------------------------
Runtime::Runtime(const Model &model, const BackendFeNN &backend)
:   m_Backend(backend), m_Model(model)
{
    m_State = m_Backend.get().createState();
}
//----------------------------------------------------------------------------
void Runtime::setInstructions(const std::vector<uint32_t> &instructions)
{
    m_State->setInstructions(instructions);
}
//----------------------------------------------------------------------------
void Runtime::allocate()
{
    // Create special array to hold field information
    m_FieldArray = m_Backend.get().createFieldArray(m_Model.get(), m_State.get());

    // Loop through state objects used by model
    for (const auto &s : m_Model.get().getStateProcesses()) {
        // Visit state process to allocate appropriate type of array
        AllocatorVisitor visitor(s.first, m_Backend.get(), m_State.get());

        // Take ownership of array and add to arrays map
        if (!m_Arrays.try_emplace(s.first, std::move(visitor.getArray())).second) {
            assert(false);
        }
    }
    
    // Loop through all the processes with fields that require population
    for(const auto &p : m_Model.get().getProcessFields()) {
        // Loop through all fields associated with process and assign corresponding arrays to file
        for(const auto &s : p.second) {
            m_FieldArray->setFieldArray(s.second, getArray(s.first));
        }
    }

    // Push populated fields to device
    m_FieldArray->pushFieldsToDevice();
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
