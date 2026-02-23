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
    URAMArray(const Type::ResolvedType &type, size_t count, DeviceFeNNSim &device)
    :   URAMArrayBase(type, count), m_Device(device)
    {
        // Allocate if count is specified
        if(count > 0) {
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
    BRAMArray(const GeNN::Type::ResolvedType &type, size_t count, DeviceFeNNSim &device)
    :   BRAMArrayBase(type, count), m_Device(device)
    {
        // Allocate if count is specified
        if(count > 0) {
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
    LLMArray(const GeNN::Type::ResolvedType &type, size_t count, DeviceFeNNSim &device)
    :   LLMArrayBase(type, count), m_Device(device)
    {
        // Allocate if count is specified
        if(count > 0) {
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
    DRAMArray(const GeNN::Type::ResolvedType &type, size_t count, DeviceFeNNSim &device)
    :   DRAMArrayBase(type, count), m_Device(device)
    {
        // Allocate if count is specified
        if(count > 0) {
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
    URAMLLMArray(const GeNN::Type::ResolvedType &type, size_t uramCount, 
                 size_t llmCount, DeviceFeNNSim &device)
    :   URAMLLMArrayBase(type, uramCount, llmCount), m_Device(device)
    {
        if(uramCount > 0) {
            // Allocate memory for host pointer
            setHostPointer(new uint8_t[getSizeBytes()]);

            // Allocate URAM
            setURAMPointer(m_Device.get().getURAMAllocator().allocate(getSizeBytes()));
        }

        // Allocate LLM
        if(llmCount > 0) {
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


//----------------------------------------------------------------------------
// SimState
//----------------------------------------------------------------------------
/*class SimState : public StateBase
{
public:
    SimState(size_t dmaBufferSize)
    :   m_DMABufferAllocator(dmaBufferSize)
    {
        m_RISCV.addCoprocessor<VectorProcessor>(vectorQuadrant);

        // Create simulated DMA controller
        m_DMAController = std::make_unique<DMAControllerSim>(m_RISCV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory(),
                                                             dmaBufferSize);
        m_RISCV.setDMAController(m_DMAController.get());
    }

    //------------------------------------------------------------------------
    // StateBase virtuals
    //------------------------------------------------------------------------
    virtual void setInstructions(const std::vector<uint32_t> &instructions) override final
    {
        m_RISCV.setInstructions(instructions);
    }

    virtual void startRun() override final
    {
        m_RISCV.setPC(0);
        m_RISCV.run();
    }

    virtual void waitRun() override final
    {
    }

    virtual std::unique_ptr<ArrayBase> createURAMArray(const GeNN::Type::ResolvedType &type, size_t count) final override
    {
        return std::make_unique<::URAMArray>(type, count, this);
    }
    
    virtual std::unique_ptr<ArrayBase> createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count) final override
    {
        return std::make_unique<::BRAMArray>(type, count, this);
    }
    
    virtual std::unique_ptr<ArrayBase> createLLMArray(const GeNN::Type::ResolvedType &type, size_t count) final override
    {
        return std::make_unique<::LLMArray>(type, count, this);
    }
    
    virtual std::unique_ptr<ArrayBase> createDRAMArray(const GeNN::Type::ResolvedType &type, size_t count) final override
    {
        return std::make_unique<::DRAMArray>(type, count, this);
    }

    virtual std::unique_ptr<ArrayBase> createURAMLLMArray(const GeNN::Type::ResolvedType &type,
                                                          size_t uramCount, size_t llmCount) final override
    {
        return std::make_unique<::URAMLLMArray>(type, uramCount, llmCount, this);
    }

    virtual std::unique_ptr<IFieldArray> createFieldArray(const Model &model) final override
    {
        return std::make_unique<::BRAMFieldArray<BRAMArray>>(GeNN::Type::Uint8, model.getNumFieldBytes(), this);
    }

    virtual std::optional<unsigned int> getSOCPower() const final override
    {
        return std::nullopt;
    }

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getRISCV() const{ return m_RISCV; }
    auto &getRISCV(){ return m_RISCV; }  

    const auto &getDMABufferAllocator() const{ return m_DMABufferAllocator; }
    auto &getDMABufferAllocator(){ return m_DMABufferAllocator; }
    
    const DMAControllerSim *getDMAController() const{ return m_DMAController.get(); }
    DMAControllerSim *getDMAController(){ return m_DMAController.get(); }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    RISCV m_RISCV;
    std::unique_ptr<DMAControllerSim> m_DMAController;
    DMABufferAllocator m_DMABufferAllocator;
};*/
}

//----------------------------------------------------------------------------
// FeNN::Backend::DeviceFeNNSim
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
DeviceFeNNSim::DeviceFeNNSim(size_t deviceIndex, size_t dmaBufferSize, ISE::SharedBusSim &sharedBus)
:   DeviceFeNN(deviceIndex), m_DMABufferAllocator(dmaBufferSize)
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
void DeviceFeNNSim::loadKernel(std::shared_ptr<const ::Model::Kernel> kernel)
{
    // **TODO** get code from runtime
    getRISCV().setInstructions(code);
}
//----------------------------------------------------------------------------
void DeviceFeNNSim::runKernel(std::shared_ptr<const ::Model::Kernel> kernel)
{
}
//----------------------------------------------------------------------------
std::unique_ptr<::Backend::ArrayBase> DeviceFeNNSim::createURAMArray(const GeNN::Type::ResolvedType &type, size_t count) const
{
}
//----------------------------------------------------------------------------
std::unique_ptr<::Backend::ArrayBase> DeviceFeNNSim::createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count) const
{
}
//----------------------------------------------------------------------------
std::unique_ptr<::Backend::ArrayBase> DeviceFeNNSim::createLLMArray(const GeNN::Type::ResolvedType &type, size_t count) const
{
}
//----------------------------------------------------------------------------
std::unique_ptr<::Backend::ArrayBase> DeviceFeNNSim::createDRAMArray(const GeNN::Type::ResolvedType &type, size_t count) const
{
}
//----------------------------------------------------------------------------
std::unique_ptr<::Backend::ArrayBase> DeviceFeNNSim::createURAMLLMArray(const GeNN::Type::ResolvedType &type,
                                                                        size_t uramCount, size_t llmCount) const
{
}

//----------------------------------------------------------------------------
// FeNN::Backend::RuntimeSim
//----------------------------------------------------------------------------
RuntimeSim::RuntimeSim(const ::Model::Model &model, size_t numCores, bool useDRAMForWeights, 
                       bool keepParamsInRegisters, Compiler::RoundingMode neuronUpdateRoundingMode, 
                       size_t dmaBufferSize)
:   Runtime(model, numCores, useDRAMForWeights, keepParamsInRegisters, neuronUpdateRoundingMode),
    m_SharedBus(numCores), m_DMABufferSize(dmaBufferSize)
{
}
//------------------------------------------------------------------------
void RuntimeSim::run(std::shared_ptr<const ::Model::Kernel> kernel)
{
    // If kernel change is required
    if(kernel != m_CurrentKernelCode) {
        // Get code
        const auto &code = getKernelCode(kernel);
        
        // Set instructions on all cores
        // **OPTIMISE** use worker threads
        for(auto &c : m_CoreState) {
            c.getRISCV().setInstructions(code);
        }
    
        // Update current kernel pointer
        m_CurrentKernelCode = kernel;
    }

    
}
//------------------------------------------------------------------------
std::unique_ptr<::Backend::DeviceBase> RuntimeSim::createDevice(size_t deviceIndex) const
{
    return std::make_unique<DeviceFeNNSim>(deviceIndex, m_DMABufferSize, m_SharedBus);
}


}