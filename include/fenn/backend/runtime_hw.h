#pragma once

// FeNN common includes
#include "fenn/common/device.h"
#include "fenn/common/dma_buffer.h"
#include "fenn/common/dma_controller.h"

// FeNN Backend includes
#include "fenn/backend/backend_export.h"
#include "fenn/backend/memory_allocator.h"
#include "fenn/backend/runtime.h"

// Forward declarations
namespace FeNN::Backend
{
class RuntimeHW;
}

//----------------------------------------------------------------------------
// FeNN::Backend::DeviceFeNNHW
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
class FENN_BACKEND_EXPORT DeviceFeNNHW : public DeviceFeNN
{
public:
    DeviceFeNNHW(size_t deviceIndex, const Runtime &runtime, 
                 Common::DMABuffer &parentDMABuffer);
    
    //------------------------------------------------------------------------
    // Device virtuals
    //------------------------------------------------------------------------
    //! Load kernel onto device
    virtual void loadKernel(std::shared_ptr<const Frontend::Kernel> kernel) override final;

    //! Run kernel on device
    virtual void runKernel(std::shared_ptr<const Frontend::Kernel> kernel) override final;

    //------------------------------------------------------------------------
    // DeviceFeNN virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::ArrayBase> createURAMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape) override final;
    virtual std::unique_ptr<Frontend::ArrayBase> createBRAMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape) override final;
    virtual std::unique_ptr<Frontend::ArrayBase> createLLMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape) override final;
    virtual std::unique_ptr<Frontend::ArrayBase> createDRAMArray(const GeNN::Type::ResolvedType &type, const Frontend::Shape &shape) override final;
    virtual std::unique_ptr<Frontend::ArrayBase> createURAMLLMArray(const GeNN::Type::ResolvedType &type,
                                                                    const Frontend::Shape &uramShape, 
                                                                    const Frontend::Shape &llmShape) override final;
    
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getDevice() const{ return m_Device; }
    auto &getDevice(){ return m_Device; }

    const auto &getDMABufferAllocator() const{ return m_DMABufferAllocator; }
    auto &getDMABufferAllocator(){ return m_DMABufferAllocator; }

    const auto &getDMABuffer() const{ return m_DMABuffer; }
    auto &getDMABuffer(){ return m_DMABuffer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Common::Device m_Device;
    Common::DMABuffer m_DMABuffer;
    DMABufferAllocator m_DMABufferAllocator;
};


//----------------------------------------------------------------------------
// FeNN::Backend::RuntimeHW
//----------------------------------------------------------------------------
class FENN_BACKEND_EXPORT RuntimeHW : public Runtime
{
public:
    RuntimeHW(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels,
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
    size_t m_DMABufferSize;
    Common::DMABuffer m_ParentDMABuffer;
};
}