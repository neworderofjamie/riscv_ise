#pragma once

// FeNN ISE includes
#include "fenn/ise/dma_controller_sim.h"
#include "fenn/ise/riscv.h"
#include "fenn/ise/shared_bus_sim.h"
#include "fenn/ise/vector_processor.h"

// FeNN Backend includes
#include "fenn/backend/backend_export.h"
#include "fenn/backend/memory_allocator.h"
#include "fenn/backend/runtime.h"

// Forward declarations
namespace FeNN::Backend
{
class RuntimeSim;
}

//----------------------------------------------------------------------------
// FeNN::Backend::DeviceFeNNSim
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class FENN_BACKEND_EXPORT DeviceFeNNSim : public DeviceFeNN
{
public:
    DeviceFeNNSim(size_t deviceIndex, Runtime &runtime, 
                  ISE::SharedBusSim &sharedBus);
    
    //------------------------------------------------------------------------
    // Device virtuals
    //------------------------------------------------------------------------
    //! Load kernel onto device
    virtual void loadKernel(std::shared_ptr<const Frontend::Kernel> kernel) override final;

    //! Run current kernel on device
    virtual void runCurrentKernel() override final;

    //------------------------------------------------------------------------
    // DeviceFeNN virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<URAMArrayBase> createURAMArray(const CompilerFrontend::Type::ResolvedType &type, const Frontend::Shape &shape) override final;
    virtual std::unique_ptr<BRAMArrayBase> createBRAMArray(const CompilerFrontend::Type::ResolvedType &type, const Frontend::Shape &shape) override final;
    virtual std::unique_ptr<LLMArrayBase> createLLMArray(const CompilerFrontend::Type::ResolvedType &type, const Frontend::Shape &shape) override final;
    virtual std::unique_ptr<DRAMArrayBase> createDRAMArray(const CompilerFrontend::Type::ResolvedType &type, const Frontend::Shape &shape) override final;
 
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getRISCV() const{ return m_RISCV; }
    auto &getRISCV(){ return m_RISCV; }  

    const auto &getDMABufferAllocator() const{ return m_DMABufferAllocator; }
    auto &getDMABufferAllocator(){ return m_DMABufferAllocator; }
    
    const auto *getDMAController() const{ return m_DMAController.get(); }
    auto *getDMAController(){ return m_DMAController.get(); }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    ISE::RISCV m_RISCV;
    std::unique_ptr<ISE::DMAControllerSim> m_DMAController;
    std::unique_ptr<ISE::RouterSim> m_Router;
    DMABufferAllocator m_DMABufferAllocator;
};

//----------------------------------------------------------------------------
// FeNN::Backend::RuntimeSim
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT RuntimeSim : public Runtime
{
public:
    RuntimeSim(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels,
               size_t numDevices, bool useDRAMForWeights = false, bool keepParamsInRegisters = true, 
               Compiler::RoundingMode neuronUpdateRoundingMode = Compiler::RoundingMode::NEAREST,
               size_t dmaBufferSize = 512 * 1024);
    
private:
    //------------------------------------------------------------------------
    // Runtime virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::DeviceBase> createDevice(size_t deviceIndex) override final;

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    ISE::SharedBusSim m_SharedBus;
};
}
