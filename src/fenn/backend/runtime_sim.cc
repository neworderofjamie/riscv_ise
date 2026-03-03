#include "fenn/backend/runtime_sim.h"

// Standard C++ includes
#include <stdexcept>

// Plog includes
#include <plog/Log.h>

// Common includes
#include "common/utils.h"

// FeNN ISE includes
#include "fenn/ise/dma_controller_sim.h"
#include "fenn/ise/riscv.h"
#include "fenn/ise/router_sim.h"
#include "fenn/ise/vector_processor.h"

// FeNN backend includes
#include "fenn/backend/memory_allocator.h"

using namespace FeNN;
using namespace FeNN::Backend;
using namespace GeNN;

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
    URAMArray(const Type::ResolvedType &type, const Frontend::Shape &shape, DeviceFeNNSim &device)
    :   URAMArrayBase(type, shape), m_Device(device)
    {
        // Allocate if count is specified
        if(getCount() > 0) {
            // Allocate memory for host pointer
            setHostPointer(new uint8_t[getSizeBytes()]);

            // Allocate URAM
            setURAMPointer(m_Device.get().getURAMAllocator().allocate(getSizeBytes()));
        }
    }
    virtual ~URAMArray()
    {
        if(getCount() > 0) {
            delete [] getHostPointer();
            setHostPointer(nullptr);
            setURAMPointer(std::nullopt);
        }
    }

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override
    {
        // Copy correct number of int16_t from host pointer to vector data memory
        auto &vectorDataMemory = m_Device.get().getRISCV().getCoprocessor<ISE::VectorProcessor>(FeNN::Common::vectorQuadrant)->getVectorDataMemory();
        std::copy_n(getHostPointer<int16_t>(), getCount(), 
                    vectorDataMemory.getData() + (getURAMPointer() / 2));
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        // Copy correct number of int16_t from vector data memory to host pointer
        const auto &vectorDataMemory = m_Device.get().getRISCV().getCoprocessor<ISE::VectorProcessor>(FeNN::Common::vectorQuadrant)->getVectorDataMemory();
        std::copy_n(vectorDataMemory.getData() + (getURAMPointer() / 2), getCount(), 
                    getHostPointer<int16_t>());
    }

private:
    std::reference_wrapper<DeviceFeNNSim> m_Device;
};

//------------------------------------------------------------------------
// BRAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in BRAM. Host memory is allocated using standard
//! allocator and is transferred to FeNN using Device functionality
class BRAMArray : public FeNN::Backend::BRAMArrayBase
{
public:
    BRAMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape, DeviceFeNNSim &device)
    :   BRAMArrayBase(type, shape), m_Device(device)
    {
        // Allocate if count is specified
        if(getCount() > 0) {
            // Allocate memory for host pointer
            setHostPointer(new uint8_t[getSizeBytes()]);

            // Allocate BRAM
            setBRAMPointer(m_Device.get().getBRAMAllocator().allocate(getSizeBytes()));
        }
    }

    virtual ~BRAMArray()
    {
        if(getCount() > 0) {
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
        // Copy correct number of int16_t from host pointer to vector data memory
        auto &scalarDataMemory = m_Device.get().getRISCV().getScalarDataMemory();
        std::copy_n(getHostPointer<uint8_t>(), getSizeBytes(), 
                    scalarDataMemory.getData() + getBRAMPointer());
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        // Copy correct number of int16_t from vector data memory to host pointer
        const auto &scalarDataMemory = m_Device.get().getRISCV().getScalarDataMemory();
        std::copy_n(scalarDataMemory.getData() + getBRAMPointer(), getSizeBytes(), 
                    getHostPointer<uint8_t>());
    }

private:
    std::reference_wrapper<DeviceFeNNSim> m_Device;
};

//------------------------------------------------------------------------
// LLMArray
//------------------------------------------------------------------------
//! Class for managing arrays in lane-local memory.
class LLMArray : public FeNN::Backend::LLMArrayBase
{
public:
    LLMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape, DeviceFeNNSim &device)
    :   LLMArrayBase(type, shape), m_Device(device)
    {
        // Allocate if count is specified
        if(getCount() > 0) {
            // Allocate memory for host pointer
            setHostPointer(nullptr);

            // Allocate LLM
            setLLMPointer(m_Device.get().getLLMAllocator().allocate(getSizeBytes()));
        }
    }

    ~LLMArray()
    {
        if(getCount() > 0) {
            delete [] getHostPointer();
            setHostPointer(nullptr);
            setLLMPointer(std::nullopt);
        }
    }

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override
    {
        LOGW << "Copying LLM buffers is implemented in simulation for convenience but is not possible on device";
        const size_t numRows = ::Common::Utils::ceilDivide(getCount(), 32);
        for(size_t l = 0; l < 32; l++) {
            auto &laneLocalMemory = m_Device.get().getRISCV().getCoprocessor<ISE::VectorProcessor>(FeNN::Common::vectorQuadrant)->getLaneLocalMemory(l);    
            int16_t *llmPointer = laneLocalMemory.getData() + (getLLMPointer() / 2);
            for(size_t r = 0; r < numRows; r++) {
                *llmPointer++ = getHostPointer<int16_t>()[(r * 32) + l];
            }
        }
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        LOGW << "Copying LLM buffers is implemented in simulation for convenience but is not possible on device";
            
        const size_t numRows = ::Common::Utils::ceilDivide(getCount(), 32);
        for(size_t l = 0; l < 32; l++) {
            const auto &laneLocalMemory = m_Device.get().getRISCV().getCoprocessor<ISE::VectorProcessor>(FeNN::Common::vectorQuadrant)->getLaneLocalMemory(l);    
            const int16_t *llmPointer = laneLocalMemory.getData() + (getLLMPointer() / 2);
            for(size_t r = 0; r < numRows; r++) {
                getHostPointer<int16_t>()[(r * 32) + l] = *llmPointer++;
            }
        }
    }

private:
    std::reference_wrapper<DeviceFeNNSim> m_Device;
};

//------------------------------------------------------------------------
// DRAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in DRAM. 
class DRAMArray : public DRAMArrayBase
{
public:
    DRAMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape, DeviceFeNNSim &device)
    :   DRAMArrayBase(type, shape), m_Device(device)
    {
        // Allocate if count is specified
        if(getCount() > 0) {
            // Allocate block of DMA buffer
            const size_t offset = m_Device.get().getDMABufferAllocator().allocate(getSizeBytes());

            // Add to virtual address of DMA buffer data to get host pointer
            setHostPointer(m_Device.get().getDMAController()->getData() + offset);

            // Use directly as device pointer
            // **NOTE** simulated DMA controller has a physical address of 0
            setDRAMPointer(offset);
        }
    }
    
    virtual ~DRAMArray()
    {
        if(getCount() > 0) {
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
    std::reference_wrapper<DeviceFeNNSim> m_Device;
};

//------------------------------------------------------------------------
// URAMLLMArray
//------------------------------------------------------------------------
//! class for arrays which are allocated in URAM but also have a delayed input in LLM
//! Typically used for implementing neuron variables with dendritically-delayed input 
class URAMLLMArray : public URAMLLMArrayBase
{
public:
    URAMLLMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &uramShape, 
                 const Frontend::Shape &llmShape, DeviceFeNNSim &device)
    :   URAMLLMArrayBase(type, uramShape, llmShape), m_Device(device)
    {
        if(getCount() > 0) {
            // Allocate memory for host pointer
            setHostPointer(new uint8_t[getSizeBytes()]);

            // Allocate URAM
            setURAMPointer(m_Device.get().getURAMAllocator().allocate(getSizeBytes()));
        }

        // Allocate LLM
        if(getLLMCount() > 0) {
            setLLMPointer(m_Device.get().getLLMAllocator().allocate(getLLMSizeBytes()));
        }
    }

    virtual ~URAMLLMArray()
    {
        if(getCount() > 0) {
            delete [] getHostPointer();
            setHostPointer(nullptr);
            setURAMPointer(std::nullopt);
        }

        if(getLLMCount() > 0) {
            setLLMPointer(std::nullopt);
        }
    }

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override
    {
        // Copy correct number of int16_t from host pointer to vector data memory
        auto &vectorDataMemory = m_Device.get().getRISCV().getCoprocessor<ISE::VectorProcessor>(FeNN::Common::vectorQuadrant)->getVectorDataMemory();
        std::copy_n(getHostPointer<int16_t>(), getCount(), 
                    vectorDataMemory.getData() + (getURAMPointer() / 2));
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        // Copy correct number of int16_t from vector data memory to host pointer
        const auto &vectorDataMemory = m_Device.get().getRISCV().getCoprocessor<ISE::VectorProcessor>(FeNN::Common::vectorQuadrant)->getVectorDataMemory();
        std::copy_n(vectorDataMemory.getData() + (getURAMPointer() / 2), getCount(), 
                    getHostPointer<int16_t>());
    }

private:
    std::reference_wrapper<DeviceFeNNSim> m_Device;
};
}

//----------------------------------------------------------------------------
// FeNN::Backend::DeviceFeNNSim
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
DeviceFeNNSim::DeviceFeNNSim(size_t deviceIndex, const Runtime &runtime, 
                             size_t dmaBufferSize, ISE::SharedBusSim &sharedBus)
:   DeviceFeNN(deviceIndex, runtime), m_DMABufferAllocator(dmaBufferSize)
{
    m_RISCV.addCoprocessor<ISE::VectorProcessor>(FeNN::Common::vectorQuadrant);

    // Create simulated DMA controller
    m_DMAController = std::make_unique<ISE::DMAControllerSim>(
        m_RISCV.getCoprocessor<ISE::VectorProcessor>(FeNN::Common::vectorQuadrant)->getVectorDataMemory(),
        dmaBufferSize);
    m_RISCV.setDMAController(m_DMAController.get());

    // Create simulated DMA controller
    m_Router = std::make_unique<ISE::RouterSim>(sharedBus, m_RISCV.getSpikeDataMemory(), deviceIndex);
    m_RISCV.setRouter(m_Router.get());
}
//----------------------------------------------------------------------------
void DeviceFeNNSim::loadKernel(std::shared_ptr<const Frontend::Kernel> kernel)
{
    // Get kernel code from runtime and load into ISE's instruction memory
    getRISCV().setInstructions(getRuntime().getKernelCode(kernel));
}
//----------------------------------------------------------------------------
void DeviceFeNNSim::runKernel(std::shared_ptr<const Frontend::Kernel> kernel)
{
    // Reset program counter and run
    m_RISCV.setPC(0);
    m_RISCV.run();
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> DeviceFeNNSim::createURAMArray(const GeNN::Type::ResolvedType &type, 
                                                                    const Frontend::Shape &shape)
{
    return std::make_unique<::URAMArray>(type, shape, *this);
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> DeviceFeNNSim::createBRAMArray(const GeNN::Type::ResolvedType &type,
                                                                    const Frontend::Shape &shape)
{
    return std::make_unique<::BRAMArray>(type, shape, *this);
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> DeviceFeNNSim::createLLMArray(const GeNN::Type::ResolvedType &type,
                                                                   const Frontend::Shape &shape)
{
    return std::make_unique<::LLMArray>(type, shape, *this);
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> DeviceFeNNSim::createDRAMArray(const GeNN::Type::ResolvedType &type,
                                                                    const Frontend::Shape &shape)
{
    return std::make_unique<::DRAMArray>(type, shape, *this);
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> DeviceFeNNSim::createURAMLLMArray(const GeNN::Type::ResolvedType &type,
                                                                        const Frontend::Shape &uramShape, 
                                                                        const Frontend::Shape &llmShape)
{
    return std::make_unique<::URAMLLMArray>(type, uramShape, llmShape, *this);
}

//----------------------------------------------------------------------------
// FeNN::Backend::RuntimeSim
//----------------------------------------------------------------------------
RuntimeSim::RuntimeSim(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels,
                      size_t numDevices, bool useDRAMForWeights, bool keepParamsInRegisters, 
                      Compiler::RoundingMode neuronUpdateRoundingMode, size_t dmaBufferSize)
:   Runtime(kernels, numDevices, useDRAMForWeights, keepParamsInRegisters, neuronUpdateRoundingMode),
    m_SharedBus(numDevices), m_DMABufferSize(dmaBufferSize)
{
}
//------------------------------------------------------------------------
std::unique_ptr<Frontend::DeviceBase> RuntimeSim::createDevice(size_t deviceIndex) const
{
    return std::make_unique<DeviceFeNNSim>(deviceIndex, *this, m_DMABufferSize, m_SharedBus);
}
}
