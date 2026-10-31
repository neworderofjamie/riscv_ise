#pragma once

// GeNN backend includes
#include "genn/backend/runtime.h"

// GeNN CUDA backend includes
#include "genn/backend_cuda/backend_cuda_export.h"

//----------------------------------------------------------------------------
// GeNN::BackendCUDA::Device
//----------------------------------------------------------------------------
namespace GeNN::BackendCUDA
{
class GENN_BACKEND_CUDA_EXPORT Device : public Backend::DeviceGeNN
{
public:
    Device(size_t deviceIndex, int deviceID, Runtime &runtime);
    virtual ~Device();
    
    //------------------------------------------------------------------------
    // Device virtuals
    //------------------------------------------------------------------------
    //! Load kernel onto device
    virtual void loadKernel(std::shared_ptr<const Frontend::Kernel> kernel) override final;

    //! Run current kernel on device
    virtual void runCurrentKernel() override final;

    //------------------------------------------------------------------------
    // DeviceGeNN virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createArray(const CompilerFrontend::Type::ResolvedType &type,
                                                             const std::vector<size_t> &shape, const std::vector<size_t> &strides) override final;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    std::pair<int, int> getComputeCapability() const;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! CUDA device
    CUdevice m_Device;
    
    //! CUDA context
    CUcontext m_Context;
    
    //! CUDA module for current kernel
    CUmodule m_CurrentKernelModule;

    //! Map of process groups to CUfunction in the current kernel
    std::unordered_map<std::shared_ptr<const Frontend::ProcessGroup>,
                       CUfunction> m_CurrentKernelProcessGroupFunctions;
};


//----------------------------------------------------------------------------
// GeNN::BackendCUDA::Runtime
//----------------------------------------------------------------------------
class GENN_BACKEND_CUDA_EXPORT Runtime : public Backend::Runtime
{
public:
    Runtime(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels, 
            const std::vector<int> &deviceIDs);


private:
    //------------------------------------------------------------------------
    // Runtime virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::DeviceBase> createDevice(size_t deviceIndex) override final;
    
    //------------------------------------------------------------------------
    // GeNN Runtime virtuals
    //------------------------------------------------------------------------
    //! Use runtime compiler to compiler kernel into PTX/other intermediate
    std::vector<char> compileKernel(const std::string &kernelCode) const override final;
    
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    
};
}