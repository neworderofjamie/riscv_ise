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


//----------------------------------------------------------------------------
// FeNN::Backend::DeviceFeNNSim
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class DeviceFeNNSim : public DeviceFeNN
{
public:
    DeviceFeNNSim(size_t deviceIndex, size_t dmaBufferSize, ISE::SharedBusSim &sharedBus);
    
    //------------------------------------------------------------------------
    // Device virtuals
    //------------------------------------------------------------------------
    //! Load kernel onto device
    virtual void loadKernel(std::shared_ptr<const ::Model::Kernel> kernel) override final;

    //! Run kernel on device
    virtual void runKernel(std::shared_ptr<const ::Model::Kernel> kernel) override final;

    //------------------------------------------------------------------------
    // DeviceFeNN virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<::Backend::ArrayBase> createURAMArray(const GeNN::Type::ResolvedType &type, size_t count) const override final;
    virtual std::unique_ptr<::Backend::ArrayBase> createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count) const override final;
    virtual std::unique_ptr<::Backend::ArrayBase> createLLMArray(const GeNN::Type::ResolvedType &type, size_t count) const override final;
    virtual std::unique_ptr<::Backend::ArrayBase> createDRAMArray(const GeNN::Type::ResolvedType &type, size_t count) const override final;
    virtual std::unique_ptr<::Backend::ArrayBase> createURAMLLMArray(const GeNN::Type::ResolvedType &type,
                                                                     size_t uramCount, size_t llmCount) const override final;
    
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
    RuntimeSim(const ::Model::Model &model, size_t numCores, bool useDRAMForWeights = false, bool keepParamsInRegisters = true, 
               Compiler::RoundingMode neuronUpdateRoundingMode = Compiler::RoundingMode::NEAREST,
               size_t dmaBufferSize = 512 * 1024);
    
    //------------------------------------------------------------------------
    // Runtime virtuals
    //------------------------------------------------------------------------
    //! Run kernel on device
    virtual void run(std::shared_ptr<const ::Model::Kernel> kernel) override final;


private:
    //------------------------------------------------------------------------
    // Runtime virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<::Backend::DeviceBase> createDevice(size_t deviceIndex) const override final;

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    ISE::SharedBusSim m_SharedBus;
    KernelPtr m_CurrentKernelCode;
    size_t m_DMABufferSize;
};
}