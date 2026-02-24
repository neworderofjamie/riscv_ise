#pragma once

// Standard C++ includes
#include <atomic>
#include <condition_variable>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cstdint>

// GeNN includes
#include "type.h"

// Backend includes
#include "backend/backend_export.h"
#include "backend/merged_model.h"

// Forward declarations
namespace Model
{
class EventContainer;
class Kernel;
class Model;
class PerformanceCounter;
class State;
class Variable;
}

//----------------------------------------------------------------------------
// FeNN::Backend::ArrayBase
//----------------------------------------------------------------------------
//! Base class for arrays created by backend
//! **NOTE** this is a temporary, simplified version of GeNN's ArrayBase
namespace Backend
{
class BACKEND_EXPORT ArrayBase
{
public:
    virtual ~ArrayBase()
    {
        m_Count = 0;
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
    const GeNN::Type::ResolvedType &getType() const{ return m_Type; }
    size_t getCount() const{ return m_Count; };
    size_t getSizeBytes() const{ return m_Count * m_Type.getValue().size; };

    //! Get array host pointer
    uint8_t *getHostPointer() const{ return m_HostPointer; }

    template<typename T>
    T *getHostPointer() const{ return reinterpret_cast<T*>(m_HostPointer); }

protected:
    ArrayBase(const GeNN::Type::ResolvedType &type, size_t count)
    :   m_Type(type), m_Count(count), m_HostPointer(nullptr)
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
    GeNN::Type::ResolvedType m_Type;
    size_t m_Count;

    uint8_t *m_HostPointer;
};

//----------------------------------------------------------------------------
// Backend::DeviceBase
//----------------------------------------------------------------------------
//! Interface Runtime classes use to communicate with state on individual devices
class BACKEND_EXPORT DeviceBase
{
public:
    DeviceBase(size_t deviceIndex)
    :   m_DeviceIndex(deviceIndex)
    {}

    virtual ~DeviceBase() = default;

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Load kernel onto device
    virtual void loadKernel(std::shared_ptr<const ::Model::Kernel> kernel) = 0;

    //! Run kernel on device
    virtual void runKernel(std::shared_ptr<const ::Model::Kernel> kernel) = 0;

    //! Create suitable array for event container on this device
    virtual std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const ::Model::EventContainer> eventContainer) const = 0;

    //! Create suitable array for performance counter on this device
    virtual std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const ::Model::PerformanceCounter> performanceCounter) const = 0;

    //! Create suitable array for variable on this device
    virtual std::unique_ptr<ArrayBase> createArray(std::shared_ptr<const ::Model::Variable> variable) const = 0;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Create array to provide storage for model state
    void createArray(std::shared_ptr<const ::Model::State> state) const;

    //! Get array associated with model state
    ArrayBase *getArray(std::shared_ptr<const ::Model::State> state) const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::unordered_map<std::shared_ptr<const ::Model::State>, std::unique_ptr<ArrayBase>> m_Arrays;
    size_t m_DeviceIndex;
    
};

//----------------------------------------------------------------------------
// IFieldArray
//----------------------------------------------------------------------------
//! Interface for object, probably backed by some sort of array,
//! that anages in-device memory fields structure
/*class IFieldArray
{
public:
    virtual ~IFieldArray() = default;

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Sets field at offset to point to array
    virtual void setFieldArray(uint32_t fieldOffset, const ArrayBase *array) = 0;

    //! Copy field data to device
    virtual void pushFieldsToDevice() = 0;
};*/


//----------------------------------------------------------------------------
// Runtime
//----------------------------------------------------------------------------
class BACKEND_EXPORT Runtime
{
public:
    Runtime(const ::Model::Model &model, size_t numDevices);
    virtual ~Runtime();

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Allocate memory for model on device
    void allocate();
   
    //! Run kernel on device
    void run(std::shared_ptr<const ::Model::Kernel> kernel);

    //std::optional<unsigned int> getSOCPower() const;
    
    size_t getNumDevices() const{ return m_NumDevices; }

protected:
    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    //! Backend-specific logic to run at beginning of allocate function
    virtual void allocatePreamble() {}

    //! Backend-specific logic to run at end of allocate function
    virtual void allocatePostamble() {}
    
    //! Set the fields associated with a merged process
    virtual void setMergedProcessFields(const MergedProcess &mergedProcess) const = 0;

    //! Create suitable device
    virtual std::unique_ptr<DeviceBase> createDevice(size_t deviceIndex) const = 0;

    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    const auto &getMergedModel() const{ return m_MergedModel; }

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
        LoadKernelCommand(std::shared_ptr<const ::Model::Kernel> kernel)
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
        std::shared_ptr<const ::Model::Kernel> m_Kernel;
    };

    //------------------------------------------------------------------------
    // RunKernelCommand
    //------------------------------------------------------------------------
    //! Command for running kernel on all devices
    class RunKernelCommand : public Command
    {
    public:
        RunKernelCommand(std::shared_ptr<const ::Model::Kernel> kernel)
        :   m_Kernel(kernel)
        {}

        //--------------------------------------------------------------------
        // Command virtuals
        //--------------------------------------------------------------------
        virtual void execute(DeviceBase *device) const override final
        {
            device->runKernel(m_Kernel);
        }

    private:
        //--------------------------------------------------------------------
        // Members
        //--------------------------------------------------------------------
        std::shared_ptr<const ::Model::Kernel> m_Kernel;
    };

    //------------------------------------------------------------------------
    // PushStateCommand
    //------------------------------------------------------------------------
    //! Command for pushing 
    class PushStateCommand : public Command
    {
    public:
        PushStateCommand(std::shared_ptr<const ::Model::State> state)
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
        std::shared_ptr<const ::Model::State> m_State;
    };

    //------------------------------------------------------------------------
    // PullStateCommand
    //------------------------------------------------------------------------
    //! Command for pushing 
    class PullStateCommand : public Command
    {
    public:
        PullStateCommand(std::shared_ptr<const ::Model::State> state)
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
        std::shared_ptr<const ::Model::State> m_State;
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

    MergedModel m_MergedModel;

    //! Current kernel loaded onto all devices
    std::shared_ptr<const ::Model::Kernel> m_CurrentKernel;

    size_t m_NumDevices;

    size_t m_WorkersReady;

    std::atomic<bool> m_WorkerRun;

    // Current command being executed by workers
    Command *m_Command;

    // Mutex and condition variables for signal
    std::mutex m_Mutex;
    std::condition_variable m_MainToWorkerCond;
    std::condition_variable m_WorkerToMainCond;
};
}