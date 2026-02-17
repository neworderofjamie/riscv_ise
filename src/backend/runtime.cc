#include "backend/runtime.h"

// Standard C++ includes
#include <functional>

// Standard C includes
#include <cstring>

// Model includes
#include "model/model.h"
#include "model/model_component.h"

namespace
{
class LambdaVariableVisitor : public Model::StateVisitor
{
    using Return = std::unique_ptr<Backend::ArrayBase>;

    template<typename T>
    using Handler = std::function<Return(std::shared_ptr<const T>)>;

    using EventContainerHandler = Handler<Model::EventContainer>;
    using PerformanceCounterHandler = Handler<Model::PerformanceCounter>;
    using VariableHandler = Handler<Model::Variable>;

public:
    LambdaVariableVisitor(std::shared_ptr<const Model::State> state,
                          EventContainerHandler handleEventContainer,
                          PerformanceCounterHandler handlePerformanceCounter,
                          VariableHandler handleVariable)
    :   m_HandleEventContainer(handleEventContainer), 
        m_HandlePerformanceCounter(handlePerformanceCounter),
        m_HandleVariable(handleVariable)
    {
        state->accept(*this);
    }

    Return &getReturn()
    {
        return m_Return;
    }

private:
    #define IMPLEMENT_VISIT(TYPE)   \
        virtual void visit(std::shared_ptr<const Model::TYPE> state) override final    \
        {                                                                           \
            m_Return = std::move(m_Handle##TYPE(state));                                          \
        }                                                                           \
        TYPE##Handler m_Handle##TYPE

    IMPLEMENT_VISIT(EventContainer);
    IMPLEMENT_VISIT(PerformanceCounter);
    IMPLEMENT_VISIT(Variable);

    #undef IMPLEMENT_VISIT

    Return m_Return;
};
}

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
//----------------------------------------------------------------------------
std::unique_ptr<ArrayBase> Runtime::createArray(std::shared_ptr<const ::Model::State> state) const
{
    LambdaVariableVisitor v(state,
                            [this](auto eventContainer)
                            {
                                return createArray(eventContainer);
                            },
                            [this](auto performanceCounter)
                            {
                                return createArray(performanceCounter);
                            },
                            [this](auto variable)
                            {
                                return createArray(variable);
                            });
    return std::move(v.getReturn());
}
}