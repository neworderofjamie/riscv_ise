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
    // **TODO** allocate BRAM array to hold process fields

    // Loop through state objects used by model
    for (const auto &s : m_Model.get().getStateProcesses()) {
        // Visit state process to allocate appropriate type of array
        AllocatorVisitor visitor(s.first, m_Backend.get(), m_State.get());

        // Take ownership of array and add to arrays map
        if (!m_Arrays.try_emplace(s.first, std::move(visitor.getArray())).second) {
            assert(false);
        }
        //s.first->
    }
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
