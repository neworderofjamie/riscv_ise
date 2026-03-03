#include "frontend/runtime.h"

// Standard C++ includes
#include <functional>

// Standard C includes
#include <cstring>

// Model includes
#include "frontend/model.h"
#include "frontend/model_component.h"
#include "frontend/shape.h"

using namespace Frontend;

namespace
{
class LambdaVariableVisitor : public StateVisitor
{
    using Return = std::unique_ptr<ArrayBase>;

    template<typename T>
    using Handler = std::function<Return(std::shared_ptr<const T>)>;

    using EventContainerHandler = Handler<EventContainer>;
    using VariableHandler = Handler<Variable>;

public:
    LambdaVariableVisitor(std::shared_ptr<const State> state,
                          EventContainerHandler handleEventContainer,
                          VariableHandler handleVariable)
    :   m_HandleEventContainer(handleEventContainer), 
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
        virtual void visit(std::shared_ptr<const TYPE> state) override final    \
        {                                                                           \
            m_Return = std::move(m_Handle##TYPE(state));                                          \
        }                                                                           \
        TYPE##Handler m_Handle##TYPE

    IMPLEMENT_VISIT(EventContainer);
    IMPLEMENT_VISIT(Variable);

    #undef IMPLEMENT_VISIT

    Return m_Return;
};
}

//--------------------------------------------------------------------------
// Frontend::ArrayBase
//--------------------------------------------------------------------------
namespace Frontend
{
void ArrayBase::memsetHostPointer(int value)
{
    std::memset(m_HostPointer, value, getSizeBytes());
}

//----------------------------------------------------------------------------
// Frontend::DeviceBase
//----------------------------------------------------------------------------
void DeviceBase::createArray(std::shared_ptr<const State> state, const Frontend::Shape &deviceShape)
{
    LambdaVariableVisitor v(state,
                            [this, &deviceShape](auto eventContainer)
                            {
                                return createArray(eventContainer, deviceShape);
                            },
                            [this, &deviceShape](auto variable)
                            {
                                return createArray(variable, deviceShape);
                            });


    // Take ownership of array and add to arrays map
    if (!m_Arrays.try_emplace(state, std::move(v.getReturn())).second) {
        throw std::runtime_error("Duplicate array found for state '" + state->getName() + "'");
    }
}
//----------------------------------------------------------------------------
ArrayBase *DeviceBase::getArray(std::shared_ptr<const State> state) const
{
    return m_Arrays.at(state).get();
}

//----------------------------------------------------------------------------
// Frontend::Runtime
//----------------------------------------------------------------------------
Runtime::~Runtime()
{
    // Join all worker threads
    m_WorkerRun = false;
    for(auto &w : m_WorkerThreads) {
        if(w.joinable()) {
            w.join();
        }
    }
}
//----------------------------------------------------------------------------
void Runtime::allocate()
{
    // Create device objects
    // **NOTE** this cannot be done in constructor as createDevice is pure-virtual
    for(size_t i = 0; i < getNumDevices(); i++) {
        m_Devices[i] = std::move(createDevice(i));
    }

    // Create special array to hold field information
    // **THINK** fields need to have been allocated at this point
    //m_FieldArray = m_State->createFieldArray(m_Model.get());
    // Perform backend-specific logic
    allocatePreamble();

    // **TODO** loop through all process groups and add performance counters

    // Loop through state objects used by model
    for (const auto &s : m_MergedModel.getModel().getStateData()) {
        // Split shape between devices as dictated by split axis
        const auto deviceShapes = s.first->getShape().split(s.second.splitDimension, 
                                                            getNumDevices());
        for(size_t i = 0; i < getNumDevices(); i++) {
            getDevices()[i]->createArray(s.first, deviceShapes[i]);
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

    // Create worker threads
    m_WorkerThreads.reserve(getNumDevices());
    for(auto &d : m_Devices) {
        m_WorkerThreads.emplace_back(&Runtime::threadFunction, this, d.get());
    }
}
//----------------------------------------------------------------------------
void Runtime::run(std::shared_ptr<const Kernel> kernel)
{
    // Check worker threads have been created
    if(m_WorkerThreads.empty()) {
        throw std::runtime_error("Cannot run command until ``allocate`` is called");
    }

    // If kernel isn't already loaded
    if(kernel != m_CurrentKernel) {
        // Run load command
        LoadKernelCommand load(kernel);
        runCommand(&load);

        // Update current kernel
        m_CurrentKernel = kernel;
    }

    // Run run command
    RunKernelCommand run(kernel);
    runCommand(&run);
}
//----------------------------------------------------------------------------
Runtime::Runtime(const Model &model, size_t numDevices)
:   m_Devices(numDevices), m_MergedModel(model), m_NumDevices(numDevices), 
    m_WorkerRun(true), m_Command(nullptr)
{
}
//----------------------------------------------------------------------------
void Runtime::runCommand(Command *command)
{
    // Set command and notify all workers
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Command = command;
        m_WorkersReady = 0;
        m_MainToWorkerCond.notify_all();
    }

    // Wait until all devices complete
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_WorkerToMainCond.wait(lock, [this](){ return m_WorkersReady == getNumDevices(); });
    }

    // Set command and notify all workers
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Command = nullptr;
        m_MainToWorkerCond.notify_all();
    }
}
//----------------------------------------------------------------------------
void Runtime::threadFunction(DeviceBase *device)
{
    // While workers should run
    while(m_WorkerRun) {
        // Wait for command
        {
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_MainToWorkerCond.wait(lock, [this](){ return m_Command != nullptr; });
        }

        // Execute command on device
        m_Command->execute(device);

        // Signal main
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_WorkersReady++;
            m_WorkerToMainCond.notify_one();
        }

        // Wait for all workers to process
        {
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_MainToWorkerCond.wait(lock, [this](){ return m_Command == nullptr; });
        }
    }
}
}
