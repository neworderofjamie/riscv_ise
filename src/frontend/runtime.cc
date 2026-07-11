#include "frontend/runtime.h"

// Standard C++ includes
#include <functional>

// Standard C includes
#include <cstring>

// Common includes
#include "common/barrier.h"
#include "common/utils.h"

// Model includes
#include "frontend/kernel.h"
#include "frontend/model.h"
#include "frontend/model_component.h"
#include "frontend/process.h"
#include "frontend/process_group.h"
#include "frontend/shape.h"

using namespace Frontend;

//----------------------------------------------------------------------------
// Frontend::MergedProcessGroup
//----------------------------------------------------------------------------
namespace Frontend
{
MergedProcessGroup::MergedProcessGroup(const Model &model, std::shared_ptr<ProcessGroup const> processGroup)
{
    // Create a hash map to group together processes with the same SHA1 digest
    std::unordered_map<boost::uuids::detail::sha1::digest_type, 
        std::vector<std::shared_ptr<Process const>>, 
        Common::Utils::SHA1Hash> protoMergedProcesses;

    // Add unmerged processes to correct vector
    for(const auto &p : processGroup->getProcesses()) {
        // Build hash digest
        boost::uuids::detail::sha1 hash;
        p->updateMergeHash(hash, model);
        const auto digest = hash.get_digest();

        // Add to map
        protoMergedProcesses[digest].push_back(p);
    }

    // Reserve final merged groups vector
    m_MergedProcesses.reserve(protoMergedProcesses.size());

    // Construct merged groups
    size_t i = 0;
    for(auto &p : protoMergedProcesses) {
        m_MergedProcesses.emplace_back(i++, p.second);

        // Add all processes in merged group to reverse lookup structure
        for(size_t j = 0; j < p.second.size(); j++) {
            const auto res = m_Destinations.try_emplace(p.second[j], 
                                                        m_MergedProcesses.back().getArchetype(), 
                                                        j);
            if(!res.second) {
                throw std::runtime_error("Process '" + p.second[j]->getName() + "' included multiple times in process group");
            }
        }
    }
}

//--------------------------------------------------------------------------
// Frontend::ArrayBase
//--------------------------------------------------------------------------
void ArrayBase::memsetHostPointer(int value)
{
    std::memset(m_HostPointer, value, getSizeBytes());
}

//----------------------------------------------------------------------------
// Frontend::DeviceBase
//----------------------------------------------------------------------------
void DeviceBase::createArray(std::shared_ptr<const State> state, const Shape &deviceShape, const Model &model)
{
    // Take ownership of array and add to arrays map
    if (!m_Arrays.try_emplace(state, std::move(state->createArray(deviceShape, model, *this))).second) {
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
    if(!m_WorkerThreads.empty()) {
	    // Clear run flag
        m_WorkerRun = false;
        m_Command = nullptr;

        // **YUCK** get all threads to loop
        m_Barrier.wait();
        m_Barrier.wait();

        // Join all worker threads
        for(auto &w : m_WorkerThreads) {
            if(w.joinable()) {
                w.join();
            }
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
    // Perform backend-specific logic
    allocatePreamble();

    // **TODO** loop through all process groups and add performance counters

    // Loop through state objects used by model
    for (const auto &s : getModel()->getStateData()) {
        // Loop through devices
        for(size_t i = 0; i < getNumDevices(); i++) {
            // Split shape and create array
            const auto deviceShape = s.first->getShape().getSplit(i, s.second.splitDimension, 
                                                               getNumDevices(), m_StateSplitGranularity);
            getDevices()[i]->createArray(s.first, deviceShape, *getModel());
        }
    }

    // Perform backend-specific logic
    allocatePostamble();

    // Create worker threads
    m_WorkerThreads.reserve(getNumDevices());
    for(size_t i = 0; i < getNumDevices(); i++) {
        m_WorkerThreads.emplace_back(&Runtime::threadFunction, this, m_Devices[i].get());
        Common::Utils::setThreadName(m_WorkerThreads.back(), "Device " + std::to_string(i) + " worker");
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
    RunCurrentKernelCommand run;
    runCommand(&run);
}
//----------------------------------------------------------------------------
void Runtime::pushStateToDevice(std::shared_ptr<const State> state)
{
    PushStateCommand push(state);
    runCommand(&push);
}
//----------------------------------------------------------------------------
void Runtime::pullStateFromDevice(std::shared_ptr<const State> state)
{
    PullStateCommand pull(state);
    runCommand(&pull);
}
//----------------------------------------------------------------------------
std::vector<ArrayBase*> Runtime::getArrays(std::shared_ptr<const State> state) const
{
    // Reserve vector of arrays
    std::vector<ArrayBase*> arrays;
    arrays.reserve(getNumDevices());

    // Get arrays from each device
    for (auto &d : getDevices()) {
        arrays.push_back(d->getArray(state));
    }

    return arrays;
}
//----------------------------------------------------------------------------
Runtime::Runtime(std::unique_ptr<Model> model, size_t numDevices, size_t stateSplitGranularity)
:   m_Devices(numDevices), m_Model(std::move(model)), m_NumDevices(numDevices), 
    m_StateSplitGranularity(stateSplitGranularity), m_WorkerRun(true), 
    m_Command(nullptr), m_Barrier(numDevices + 1)
{
    // Loop through kernels
    for(const auto &k : getModel()->getKernels()) {
        // Loop through all process groups in kernel
        const auto processGroups = k->getAllProcessGroups();
        for (const auto &g : processGroups) {
            // Create merged process group for each process group
            if (!m_MergedProcessGroups.try_emplace(g, *getModel(), g).second) {
                throw std::runtime_error("Process group '" + g->getName() + "' referenced multiples time in model");
            }
        }
    }
}
//----------------------------------------------------------------------------
void Runtime::runCommand(Command *command)
{
    // Set command
    // **NOTE** all workers should be waiting for barrier at this point
    m_Command = command;

    // Wait for all workers to be ready
    m_Barrier.wait();

    // Wait for all workers to finish
    m_Barrier.wait();   
   
}
//----------------------------------------------------------------------------
void Runtime::threadFunction(DeviceBase *device)
{
    // While workers should run
    while(m_WorkerRun) {
        // Wait for command
        m_Barrier.wait();

        // Execute command on device
        if(m_Command != nullptr) {
            m_Command->execute(device);
        }

        // Wait for all workers to complete
        m_Barrier.wait();
    }
}
}
