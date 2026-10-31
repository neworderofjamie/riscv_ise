#include "genn/backend_cuda/runtime.h"

// Standard C++ includes
#include <optional>
#include <stdexcept>

// CUDA includes
#include <cuda.h>
#include <nvJitLink.h>
#include <nvrtc.h>

// PLOG includes
#include <plog/Log.h>

// Compiler frontend includes
#include "compiler_frontend/type.h"

// GeNN CUDA backend includes
#include "genn/backend_cuda/utils.h"

using namespace GeNN;
using namespace CompilerFrontend;

//------------------------------------------------------------------------
// Anonymous namespace
//------------------------------------------------------------------------
namespace
{
//------------------------------------------------------------------------
// Array
//------------------------------------------------------------------------
class Array : public Frontend::ArrayBase
{
public:
    Array(const Type::ResolvedType &type, const std::vector<size_t> &shape,
          const std::vector<size_t> &strides, Device &device)
    :   Frontend::ArrayBase(type, shape, strides), m_Device(device)
    {
        // Allocate if count is specified
        if(getSizeBytes() > 0) {
            // Allocate host memory
            // **TODO** zero copy - where do the flags go in driver API?
            std::byte *hostPointer = nullptr;
            CHECK_CU_ERRORS(cuMemAllocHost(&hostPointer, getSizeBytes()));
            setHostPointer(hostPointer);

            // Allocate device memory
            // **TODO** stride etc?
            CHECK_CU_ERRORS(cuMemAlloc(&m_DevicePointer, getSizeBytes()));
        }
    }
    virtual ~Array()
    {
        if(getSizeBytes() > 0) {
            CHECK_CU_ERRORS(cuMemFreeHost(hostPointer));
            setHostPointer(nullptr);
            
            CHECK_CU_ERRORS(cuMemFree(m_DevicePointer));
            m_DevicePointer = nullptr;
        }
    }

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override
    {
        if(getSizeBytes() > 0) {
            CHECK_CU_ERRORS(cuMemcpy(m_DevicePointer, getHostPointer(), getSizeBytes()));
        }
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        if(getSizeBytes() > 0) {
            CHECK_CU_ERRORS(cuMemcpy(getHostPointer(), m_DevicePointer, getSizeBytes()));
        }
    }

private:
    std::reference_wrapper<DeviceFeNNHW> m_Device;
    uint8_t *m_DevicePointer;
};
}

//----------------------------------------------------------------------------
// GeNN::BackendCUDA::Device
//----------------------------------------------------------------------------
namespace GeNN::BackendCUDA
{
Device::Device(size_t deviceIndex, int deviceID, Runtime &runtime)
:   Backend::Device(deviceIndex, runtime), m_Device(nullptr), m_Context(nullptr), 
    m_CurrentKernelModule(nullptr)
{
    // Get device and create context
    CHECK_CU_ERRORS(cuDeviceGet(&m_Device, deviceID));
    CHECK_CU_ERRORS(cuCtxCreate(&m_Context, NULL, 0, m_Device));
}
//----------------------------------------------------------------------------
Device::~Device()
{
    // If we have loaded a CUDA module, unload it
    if(m_CurrentKernelModule) {
        CHECK_CU_ERRORS(cuModuleUnload(m_CurrentKernelModule));
    }

    // Destroy context
    CHECK_CU_ERRORS(cuCtxDestroy(m_Context));

}
//----------------------------------------------------------------------------
void Device::loadKernel(std::shared_ptr<const Frontend::Kernel> kernel)
{
    const auto &code = getRuntime().getKernelCode(kernel);
    
    const auto [major, minor] = getComputeCapability();
    const int arch = (major * 10) + minor;
    const char* linkOptions[] = { "-arch=sm_80" };

    // Create JIT linker
    nvJitLinkHandle linker;
    CHECK_NVJITLINK_ERRORS(linker, 
                           nvJitLinkCreate(&linker, 1, linkOptions));

    // Loop through all chunks of PTX 
    for(const auto &c : code) {
        CHECK_NVJITLINK_ERRORS(linker, 
                               nvJitLinkAddData(linker, NVJITLINK_INPUT_PTX, c.second.data(), c.size()));
    }
    
    // Finalise linking
    CHECK_NVJITLINK_ERRORS(linker, 
                           nvJitLinkComplete(linker));
                           

    // Get size of cubin
    size_t cubinSize;
    CHECK_NVJITLINK_ERRORS(handle, nvJitLinkGetLinkedCubinSize(linker, &cubinSize));
    
    // Allocate temporary memory
    std::vector<std::byte> cubin(cubinSize);
    CHECK_NVJITLINK_ERRORS(linker, nvJitLinkGetLinkedCubin(linker, cubin.data()));
    
    // Destroy linker - we're done with it!
    CHECK_NVJITLINK_ERRORS(linker, nvJitLinkDestroy(&linker));

    // If we have loaded a CUDA module, unload it
    if(m_CurrentKernelModule) {
        CHECK_CU_ERRORS(cuModuleUnload(m_CurrentKernelModule));
    }
    
    // Load cubin data into module
    CHECK_CU_ERRORS(cuModuleLoadData(&m_CurrentKernelModule, cubin.data()));
    
    // Clear any ex
    m_CurrentKernelProcessGroupFunctions.clear();
    
    std::transform(c.cbegin(), c.end(),
    
    CHECK_CU_ERRORS(cuModuleGetFunction(&kernel, module, "saxpy"));

}
//----------------------------------------------------------------------------
void Device::runCurrentKernel()
{
    // **TODO** call into backend-specific kernel class with lambda function to actually launch kernels
    // Wait until ready flag
    //LOGD_FENN_BACKEND << "Running";
    //m_Device.waitOnNonZero(0);
    //LOGD_FENN_BACKEND << "Done";
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> Device::createArray(const Type::ResolvedType &type, 
                                                         const std::vector<size_t> &shape,
                                                         const std::vector<size_t> &strides)
{
    // Set context and create array
    // **NOTE** this is called from main thread so this is important!
    CHECK_CU_ERRORS(cuCtxSetCurrent(m_Context));
    return std::make_unique<::Array>(type, shape, strides, *this);
}
//----------------------------------------------------------------------------
std::tuple<int, int> Device::getComputeCapability() const
{
    // Get major and minor
    int major = 0;
    int minor = 0;
    CHECK_CU_ERRORS(cuDeviceGetAttribute(&major,
                    CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, m_Device));
    CHECK_CU_ERRORS(cuDeviceGetAttribute(&minor,
                    CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, m_Device));

    return std::make_tuple(major, minor);
}
//----------------------------------------------------------------------------
// GeNN::BackendCUDA::Runtime
//----------------------------------------------------------------------------
Runtime::Runtime(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels,
                 const std::vector<int> &deviceIDs)
:   Runtime(kernels, deviceIDs)
{
    // Initialise CUDA 
    // **NOTE** SO suggests it's non-problematic to call multiple times
    CHECK_CU_ERRORS(cuInit(0));
}
//------------------------------------------------------------------------
std::unique_ptr<Frontend::DeviceBase> Runtime::createDevice(size_t deviceIndex)
{
    return std::make_unique<Device>(deviceIndex, getDeviceIDs.at(deviceIndex), *this);
}
}