#include "fenn/backend/runtime_hw.h"

// Standard C++ includes
#include <optional>
#include <stdexcept>

// PLOG includes
#include <plog/Log.h>

// Compiler frontend includes
#include "compiler_frontend/type.h"

// FeNN common includes
#include "fenn/common/device.h"
#include "fenn/common/dma_buffer.h"
#include "fenn/common/dma_controller.h"

// FeNN backend includes
#include "fenn/backend/memory_allocator.h"

using namespace FeNN;
using namespace FeNN::Backend;
using namespace CompilerFrontend;

//------------------------------------------------------------------------
// Anonymous namespace
//------------------------------------------------------------------------
namespace
{
//------------------------------------------------------------------------
// URAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in URAM. Host memory lives in 
//! (uncached) DMA buffer and is transferred to FeNN using DMA controller
class URAMArray : public URAMArrayBase
{
public:
    URAMArray(const Type::ResolvedType &type, const std::vector<size_t> &shape,
              const std::vector<size_t> &strides, DeviceFeNNHW &device)
    :   URAMArrayBase(type, shape, strides), m_Device(device)
    {
        // Allocate if count is specified
        if(getSizeBytes() > 0) {
            // Allocate block of DMA buffer and set host pointer
            m_DMABufferOffset = m_Device.get().getDMABufferAllocator().allocate(getSizeBytes());
            setHostPointer(m_Device.get().getDMABuffer().getData() + m_DMABufferOffset.value());

            // **TODO** zero DMA buffer

            // Allocate URAM
            setURAMPointer(m_Device.get().getURAMAllocator().allocate(getSizeBytes()));
        }
    }
    virtual ~URAMArray()
    {
        if(getSizeBytes() > 0) {
            setHostPointer(nullptr);
            setURAMPointer(std::nullopt);
            m_DMABufferOffset.reset();
        }
    }

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override
    {
        if(getSizeBytes() > 0) {
            // Start DMA write and wait for completion
            auto *dmaController = m_Device.get().getDevice().getDMAController();
            dmaController->startWrite(getURAMPointer().value(), m_Device.get().getDMABuffer(),
                                      m_DMABufferOffset.value(), getSizeBytes());
            dmaController->waitForWriteComplete();
        }
    }

    virtual void memsetHostPointer(int value) final override
    {
        // **YUCK** memset seems to do something that doens't play
        // nicely with mmap'd memory and causes bus error
        for (size_t i = 0; i < getSizeBytes(); i++) {
            getHostPointer()[i] = value;
        }
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        // Start DMA read and wait for completion
        if(getSizeBytes() > 0) {
            auto *dmaController = m_Device.get().getDevice().getDMAController();
            dmaController->startRead(m_Device.get().getDMABuffer(), m_DMABufferOffset.value(), 
                                     getURAMPointer().value(), getSizeBytes());
            dmaController->waitForReadComplete();
        }
    }

private:
    std::optional<size_t> m_DMABufferOffset;
    std::reference_wrapper<DeviceFeNNHW> m_Device;
};

//------------------------------------------------------------------------
// BRAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in BRAM. Host memory is allocated using standard
//! allocator and is transferred to FeNN using Device functionality
class BRAMArray : public BRAMArrayBase
{
public:
    BRAMArray(const Type::ResolvedType &type, const std::vector<size_t> &shape,
              const std::vector<size_t> &strides, DeviceFeNNHW &device)
    :   BRAMArrayBase(type, shape, strides), m_Device(device)
    {
        // Allocate if count is specified
        if(getSizeBytes() > 0) {
            // Allocate memory for host pointer, padding to match BRAM alignement
            // **NOTE** zero so padding around strides doesn't contain random values
            setHostPointer(new uint8_t[::Common::Utils::padSize(getSizeBytes(), 4)]());

            // Allocate BRAM
            setBRAMPointer(m_Device.get().getBRAMAllocator().allocate(getSizeBytes()));
        }
    }

    virtual ~BRAMArray()
    {
        if(getSizeBytes() > 0) {
            delete [] getHostPointer();
            setHostPointer(nullptr);
            setBRAMPointer(std::nullopt);
        }
    }

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override
     {
        if(getSizeBytes() > 0) {
            m_Device.get().getDevice().memcpyDataToDevice(getBRAMPointer().value(), getHostPointer(),
                                                          getSizeBytes());
        }
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        if(getSizeBytes() > 0) {
            m_Device.get().getDevice().memcpyDataFromDevice(getHostPointer(), getBRAMPointer().value(),
                                                            getSizeBytes());
        }
    }
private:
     std::reference_wrapper<DeviceFeNNHW> m_Device;
};

//------------------------------------------------------------------------
// LLMArray
//------------------------------------------------------------------------
//! Class for managing arrays in lane-local memory.
class LLMArray : public LLMArrayBase
{
public:
    LLMArray(const Type::ResolvedType &type, const std::vector<size_t> &shape,
             const std::vector<size_t> &strides, DeviceFeNNHW &device)
    :   LLMArrayBase(type, shape, strides), m_Device(device)
    {
        // Allocate if count is specified
        if(getSizeBytes() > 0) {
            // Don't allocate memory for host pointer
            setHostPointer(nullptr);

            // Allocate LLM
            setLLMPointer(m_Device.get().getLLMAllocator().allocate(getSizeBytes()));
        }
    }

    virtual ~LLMArray()
    {
        if(getSizeBytes() > 0) {
            setLLMPointer(std::nullopt);
        }
    }
    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override
    {
        throw std::runtime_error("LLM arrays cannot be pushed to device on FeNN");
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        throw std::runtime_error("LLM arrays cannot be pulled from device on FeNN");
    }
private:
    std::reference_wrapper<DeviceFeNNHW> m_Device;
};

//------------------------------------------------------------------------
// DRAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in DRAM. 
class DRAMArray : public DRAMArrayBase
{
public:
    DRAMArray(const Type::ResolvedType &type, const std::vector<size_t> &shape,
              const std::vector<size_t> &strides, DeviceFeNNHW &device)
    :   DRAMArrayBase(type, shape, strides), m_Device(device)
    {
        // Allocate if count is specified
        if(getSizeBytes() > 0) {
            // Allocate block of DMA buffer
            const size_t offset = m_Device.get().getDMABufferAllocator().allocate(getSizeBytes());

            // Add to virtual address of DMA buffer data to get host pointer
            setHostPointer(m_Device.get().getDMABuffer().getData() + offset);

            // **TODO** zero DMA buffer

            // Add to physical address of DMA buffer to get device pointer
            setDRAMPointer(m_Device.get().getDMABuffer().getPhysicalAddress() + offset);
        }
    }

    virtual ~DRAMArray()
    {
        if(getSizeBytes() > 0) {
            // **NOTE** no memory is owned by array so just invalidate
            setHostPointer(nullptr);
            setDRAMPointer(std::nullopt);
        }
    }
    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override
    {
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
    }

private:
    std::reference_wrapper<DeviceFeNNHW> m_Device;
};
}

//----------------------------------------------------------------------------
// FeNN::Backend::DeviceFeNNHW
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
DeviceFeNNHW::DeviceFeNNHW(size_t deviceIndex, Runtime &runtime, 
                           Common::DMABuffer &parentDMABuffer)
:   DeviceFeNN(deviceIndex, runtime), m_Device(deviceIndex, runtime.getNumDevices()),
    m_DMABuffer(parentDMABuffer, 
                (runtime.getNumDevices() == 1) ? parentDMABuffer.getPhysicalAddress() : (0x40000000 + (deviceIndex * 0x10000000)),
                (runtime.getNumDevices() == 1) ? (parentDMABuffer.getPhysicalAddress() + parentDMABuffer.getSize()) : (0x50000000 + (deviceIndex * 0x10000000))),
    m_DMABufferAllocator(m_DMABuffer.getSize())
{
    if(m_DMABuffer.getSize() < runtime.getDMABufferSize()) {
        LOGW_FENN_BACKEND << "DMA buffer created by UDMABuf driver not as large as requested buffer size";
    }
}
//----------------------------------------------------------------------------
void DeviceFeNNHW::loadKernel(std::shared_ptr<const Frontend::Kernel> kernel)
{
    // Get kernel code from runtime and upload
    m_Device.uploadCode(getRuntime().getKernelCode(kernel));
}
//----------------------------------------------------------------------------
void DeviceFeNNHW::runCurrentKernel()
{
    if (getDeviceIndex() == 0) {
        static_cast<RuntimeHW&>(getRuntime()).getDeviceControl().setEnabled(true);
    }

    LOGD_FENN_BACKEND << "Running";

     // Wait until ready flag
    m_Device.waitOnNonZero(0);
    LOGD_FENN_BACKEND << "Done";

    // Disable core
    if (getDeviceIndex() == 0) {
        static_cast<RuntimeHW&>(getRuntime()).getDeviceControl().setEnabled(false);
    }
}
//----------------------------------------------------------------------------
std::unique_ptr<URAMArrayBase> DeviceFeNNHW::createURAMArray(const Type::ResolvedType &type, 
                                                             const std::vector<size_t> &shape,
                                                             const std::vector<size_t> &strides)
{
    return std::make_unique<::URAMArray>(type, shape, strides, *this);
}
//----------------------------------------------------------------------------
std::unique_ptr<BRAMArrayBase> DeviceFeNNHW::createBRAMArray(const Type::ResolvedType &type,
                                                             const std::vector<size_t> &shape,
                                                             const std::vector<size_t> &strides)
{
    return std::make_unique<::BRAMArray>(type, shape, strides, *this);
}
//----------------------------------------------------------------------------
std::unique_ptr<LLMArrayBase> DeviceFeNNHW::createLLMArray(const Type::ResolvedType &type,
                                                           const std::vector<size_t> &shape,
                                                           const std::vector<size_t> &strides)
{
    return std::make_unique<::LLMArray>(type, shape, strides, *this);
}
//----------------------------------------------------------------------------
std::unique_ptr<DRAMArrayBase> DeviceFeNNHW::createDRAMArray(const Type::ResolvedType &type,
                                                             const std::vector<size_t> &shape,
                                                             const std::vector<size_t> &strides)
{
    return std::make_unique<::DRAMArray>(type, shape, strides, *this);
}

//----------------------------------------------------------------------------
// FeNN::Backend::RuntimeHW
//----------------------------------------------------------------------------
RuntimeHW::RuntimeHW(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels,
                     size_t numDevices, bool useDRAMForWeights, bool keepParamsInRegisters, 
                     Compiler::RoundingMode neuronUpdateRoundingMode, size_t dmaBufferSize)
:   Runtime(kernels, numDevices, false, useDRAMForWeights, keepParamsInRegisters, neuronUpdateRoundingMode, dmaBufferSize),
    m_DeviceControl(numDevices)
{
}
//------------------------------------------------------------------------
std::unique_ptr<Frontend::DeviceBase> RuntimeHW::createDevice(size_t deviceIndex)
{
    return std::make_unique<DeviceFeNNHW>(deviceIndex, *this, m_ParentDMABuffer);
}
}