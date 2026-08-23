#pragma once

// Standard C++ includes
#include <atomic>
#include <memory>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cstdint>

// Common includes
#include "common/barrier.h"

// Compiler frontend includes
#include "compiler_frontend/type.h"

// Frontend includes
#include "frontend/frontend_export.h"
#include "frontend/shape.h"

// Forward declarations
namespace Frontend
{
class Kernel;
class Process;
class ProcessGroup;
class Model;
class State;
}

//----------------------------------------------------------------------------
// Frontend::MergedProcess
//----------------------------------------------------------------------------
namespace Frontend
{
template<typename T>
class Merged
{
public:
    Merged(size_t index, const std::vector<std::shared_ptr<const T>> &merged)
        :   m_Index(index), m_Merged(merged)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    size_t getIndex() const { return m_Index; }

    //! Get 'archetype' process - it's properties represent those of all other merged processes
    const auto &getArchetype() const { return m_Merged.front(); }

    //! Get 'archetype' process - it's properties represent those of all other merged processes
    template<typename P>
    const auto getArchetype() const 
    { 
        return std::dynamic_pointer_cast<const P>(getArchetype());
    }

    //! Get vector of merged processes
    const auto &getMerged() const{ return m_Merged; }

    template<typename P = Process, typename F>
    void forEachMerged(F func) const
    {
        // Loop through processes
        for (const auto &p : getMerged()) {
            // Cast to desired type
            auto derivedP = std::dynamic_pointer_cast<const P>(p);
            assert(derivedP);

            // Call function
            func(derivedP);
        }
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    size_t m_Index;
    std::vector<std::shared_ptr<T const>> m_Merged;
};

using MergedProcess = Merged<Process>;

//----------------------------------------------------------------------------
// Frontend::MergedProcessGroup
//----------------------------------------------------------------------------
class FRONTEND_EXPORT MergedProcessGroup
{
public:
    //! The archetype process it has been merged with and
    using Destination = std::pair<std::shared_ptr<Process const>, size_t>;

    MergedProcessGroup(const Model &model, std::shared_ptr<const ProcessGroup> processGroup);

    //----------------------------------------------------------------------------
    // Public API
    //----------------------------------------------------------------------------
    //! Get vector of merged processes within this groups
    const auto &getMergedProcesses() const{ return m_MergedProcesses; }

    //! Determine which merged process a process has ended up in following merging
    const Destination &getDestination(std::shared_ptr<Process const> process) const
    {
        return m_Destinations.at(process);
    }

private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    std::vector<MergedProcess> m_MergedProcesses;

    // Reverse look up structure to find where process ended up in merging process
    std::unordered_map<std::shared_ptr<Process const>, Destination> m_Destinations;
};

//----------------------------------------------------------------------------
// Frontend::ArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays created by backend
//! **NOTE** this is a temporary, simplified version of GeNN's ArrayBase
class FRONTEND_EXPORT ArrayBase
{
public:
    virtual ~ArrayBase()
    {
    }

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() = 0;

    //! Copy entire array from device
    virtual void pullFromDevice() = 0;

    //! Serialise backend-specific device object to uint32_t
    virtual void serialiseDeviceObject(std::vector<std::byte> &bytes) const = 0;

    //! Memset the host pointer
    virtual void memsetHostPointer(int value);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const CompilerFrontend::Type::ResolvedType &getType() const{ return m_Type; }
    const auto &getShape() const{ return m_Shape; }
    const auto &getStrides() const{ return m_Strides; }
    size_t getSizeBytes() const{ return m_SizeBytes; };

    //! Get array host pointer
    uint8_t *getHostPointer() const{ return m_HostPointer; }

    template<typename T>
    T *getHostPointer() const{ return reinterpret_cast<T*>(m_HostPointer); }

protected:
    ArrayBase(const CompilerFrontend::Type::ResolvedType &type,
              const std::vector<size_t> &shape, const std::vector<size_t> &strides)
    :   m_Type(type), m_SizeBytes(shape.front() * strides.front()), m_Shape(shape), 
        m_Strides(strides), m_HostPointer(nullptr)
    {
    }

    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------    
    void setHostPointer(uint8_t *hostPointer) { m_HostPointer = hostPointer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    CompilerFrontend::Type::ResolvedType m_Type;
    size_t m_SizeBytes;
    std::vector<size_t> m_Shape;
    std::vector<size_t> m_Strides;

    uint8_t *m_HostPointer;
};

//----------------------------------------------------------------------------
// Frontend::DeviceBase
//----------------------------------------------------------------------------
//! Interface Runtime classes use to communicate with state on individual devices
class FRONTEND_EXPORT DeviceBase
{
public:
    DeviceBase(size_t deviceIndex)
    :   m_DeviceIndex(deviceIndex)
    {}
    
    DeviceBase(const DeviceBase&) = delete;
    DeviceBase(DeviceBase&&) = delete;

    virtual ~DeviceBase() = default;

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Load kernel onto device
    virtual void loadKernel(std::shared_ptr<const Kernel> kernel) = 0;

    //! Run current kernel on device
    virtual void runCurrentKernel() = 0;

    //! Create suitable array for performance counter on this device
    virtual std::unique_ptr<ArrayBase> createPerformanceCounter() = 0;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Create array to provide storage for model state
    void createArray(std::shared_ptr<const State> state, std::optional<size_t> splitDimension,
                     uint32_t indexDimensions, size_t numDevices, const Model &model);

    //! Get array associated with model state
    ArrayBase *getArray(std::shared_ptr<const State> state) const;
    
    size_t getDeviceIndex() const{ return m_DeviceIndex; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unordered_map<std::shared_ptr<const State>, std::unique_ptr<ArrayBase>> m_Arrays;
    size_t m_DeviceIndex;
};

//----------------------------------------------------------------------------
// Frontend::Runtime
//----------------------------------------------------------------------------
class FRONTEND_EXPORT Runtime
{
public:
    virtual ~Runtime();

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Allocate memory for model on device
    void allocate();
   
    //! Run kernel on device
    void run(std::shared_ptr<const Kernel> kernel);

    //! Push state to all devices
    void pushStateToDevice(std::shared_ptr<const State> state);

    //! Pull state from all devices
    void pullStateFromDevice(std::shared_ptr<const State> state);

    //! Get array(s) associated with state
    std::vector<ArrayBase*> getArrays(std::shared_ptr<const State> state) const;
    
    //! Get size and shape of array state will map to on given device
    std::tuple<std::vector<size_t>, std::vector<size_t>> getDeviceArrayShapeStrides(std::shared_ptr<const State> state, 
                                                                                    size_t deviceIndex) const;

    std::optional<size_t> getSplitDimension(std::shared_ptr<const State> state) const;

    size_t getNumDevices() const{ return m_NumDevices; }

    //! Get map of process groups to merged processes
    const auto &getMergedProcessGroups() const{ return m_MergedProcessGroups; }

    template<typename T = Model>
    const T *getModel() const
    {
        return static_cast<const T*>(m_Model.get());
    }

protected:
    Runtime(std::unique_ptr<Model> model, size_t numDevices);
    
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Backend-specific logic to run at beginning of allocate function
    virtual void allocatePreamble() {}

    //! Backend-specific logic to run at end of allocate function
    virtual void allocatePostamble() {}

    //! Create suitable device
    virtual std::unique_ptr<DeviceBase> createDevice(size_t deviceIndex) = 0;

    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    const auto &getDevices() const{ return m_Devices; }
    auto &getDevices(){ return m_Devices; }

private:
    //------------------------------------------------------------------------
    // Command
    //------------------------------------------------------------------------
    //! Base class for commands to run on worker threads
    class Command
    {
    public:
        //--------------------------------------------------------------------
        // Declared virtuals
        //--------------------------------------------------------------------
        virtual void execute(DeviceBase *device) const = 0;
    };

    //------------------------------------------------------------------------
    // LoadKernelCommand
    //------------------------------------------------------------------------
    //! Command for loading kernels onto devices
    class LoadKernelCommand : public Command
    {
    public:
        LoadKernelCommand(std::shared_ptr<const Kernel> kernel)
        :   m_Kernel(kernel)
        {}

        //--------------------------------------------------------------------
        // Command virtuals
        //--------------------------------------------------------------------
        virtual void execute(DeviceBase *device) const override final
        {
            device->loadKernel(m_Kernel);
        }

    private:
        //--------------------------------------------------------------------
        // Members
        //--------------------------------------------------------------------
        std::shared_ptr<const Kernel> m_Kernel;
    };

    //------------------------------------------------------------------------
    // RunCurrentKernelCommand
    //------------------------------------------------------------------------
    //! Command for running kernel on all devices
    class RunCurrentKernelCommand : public Command
    {
    public:
        //--------------------------------------------------------------------
        // Command virtuals
        //--------------------------------------------------------------------
        virtual void execute(DeviceBase *device) const override final
        {
            device->runCurrentKernel();
        }
    };

    //------------------------------------------------------------------------
    // PushStateCommand
    //------------------------------------------------------------------------
    //! Command for pushing 
    class PushStateCommand : public Command
    {
    public:
        PushStateCommand(std::shared_ptr<const State> state)
        :   m_State(state)
        {}

        //--------------------------------------------------------------------
        // Command virtuals
        //--------------------------------------------------------------------
        virtual void execute(DeviceBase *device) const override final
        {
            device->getArray(m_State)->pushToDevice();
        }

    private:
        //--------------------------------------------------------------------
        // Members
        //--------------------------------------------------------------------
        std::shared_ptr<const State> m_State;
    };

    //------------------------------------------------------------------------
    // PullStateCommand
    //------------------------------------------------------------------------
    //! Command for pushing 
    class PullStateCommand : public Command
    {
    public:
        PullStateCommand(std::shared_ptr<const State> state)
        :   m_State(state)
        {}

        //--------------------------------------------------------------------
        // Command virtuals
        //--------------------------------------------------------------------
        virtual void execute(DeviceBase *device) const override final
        {
            device->getArray(m_State)->pullFromDevice();
        }

    private:
        //--------------------------------------------------------------------
        // Members
        //--------------------------------------------------------------------
        std::shared_ptr<const State> m_State;
    };

    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    //! Run command on all worker threads
    void runCommand(Command *command);

    //! Thread function run on each worker thread to execute commands on device
    void threadFunction(DeviceBase *device);

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::vector<std::unique_ptr<DeviceBase>> m_Devices;
    
    // Worker threads
    std::vector<std::thread> m_WorkerThreads;

    // Model
    std::unique_ptr<Model> m_Model;

    //! Current kernel loaded onto all devices
    std::shared_ptr<const Kernel> m_CurrentKernel;

    size_t m_NumDevices;

    std::atomic<bool> m_WorkerRun;

    // Current command being executed by workers
    Command *m_Command;

    // Barrier used for synchronising command execution
    Common::Barrier m_Barrier;

    // Map of process groups to merged process groups
    std::unordered_map<std::shared_ptr<ProcessGroup const>, MergedProcessGroup> m_MergedProcessGroups;
};
}
