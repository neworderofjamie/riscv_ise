#pragma once

// FeNN common includes
#include "fenn/common/device.h"
#include "fenn/common/device_control.h"
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
    DeviceFeNNHW(size_t deviceIndex, Runtime &runtime, 
                 Common::DMABuffer &parentDMABuffer);
    
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
    virtual std::unique_ptr<URAMArrayBase> createURAMArray(const CompilerFrontend::Type::ResolvedType &type,
                                                           const std::vector<size_t> &shape, const std::vector<size_t> &strides) override final;
    virtual std::unique_ptr<BRAMArrayBase> createBRAMArray(const CompilerFrontend::Type::ResolvedType &type,
                                                           const std::vector<size_t> &shape, const std::vector<size_t> &strides) override final;
    virtual std::unique_ptr<LLMArrayBase> createLLMArray(const CompilerFrontend::Type::ResolvedType &type,
                                                         const std::vector<size_t> &shape, const std::vector<size_t> &strides) override final;
    virtual std::unique_ptr<DRAMArrayBase> createDRAMArray(const CompilerFrontend::Type::ResolvedType &type,
                                                           const std::vector<size_t> &shape, const std::vector<size_t> &strides) override final;

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
    
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getDeviceControl() const{ return m_DeviceControl; }
    auto &getDeviceControl(){ return m_DeviceControl; }

protected:
    //------------------------------------------------------------------------
    // RunCurrentKernelCommand
    //------------------------------------------------------------------------
    //! Command for running kernel on all devices
    class RunCurrentKernelCommand : public Frontend::Runtime::RunCurrentKernelCommand
    {
    public:
        //--------------------------------------------------------------------
        // Command virtuals
        //--------------------------------------------------------------------
        //! Run any code required before the command execures on the main thread
        virtual void preamble(Frontend::Runtime &runtime) const override final
        {
            // Hand DMA buffer over to FeNN cores before running kernel
            static_cast<RuntimeHW&>(runtime).setDMABufferAccessMode(Common::DMABuffer::AccessMode::FENN);
        }

        //! Run any code required after the command execures on the main thread
        virtual void postamble(Frontend::Runtime &runtime) const override final
        {
            // Once kernel is finished, hand DMA buffer back to CPU
            static_cast<RuntimeHW&>(runtime).setDMABufferAccessMode(Common::DMABuffer::AccessMode::CPU);
        }
    };

    //------------------------------------------------------------------------
    // PushStateCommand
    //------------------------------------------------------------------------
    //! Command for pushing 
    class PushStateCommand : public Frontend::Runtime::PushStateCommand
    {
    public:
        using Frontend::Runtime::PushStateCommand::PushStateCommand;

        //--------------------------------------------------------------------
        // Command virtuals
        //--------------------------------------------------------------------
        //! Run any code required before the command execures on the main thread
        virtual void preamble(Frontend::Runtime &runtime) const override final
        {
            // Hand DMA buffer over to FeNN cores before starting DMA
            static_cast<RuntimeHW&>(runtime).setDMABufferAccessMode(Common::DMABuffer::AccessMode::FENN);
        }

        //! Run any code required after the command execures on the main thread
        virtual void postamble(Frontend::Runtime &runtime) const override final
        {
            // Once DMA is finished, hand DMA buffer back to CPU
            static_cast<RuntimeHW&>(runtime).setDMABufferAccessMode(Common::DMABuffer::AccessMode::CPU);
        }
    };

    //------------------------------------------------------------------------
    // PullStateCommand
    //------------------------------------------------------------------------
    //! Command for pushing 
    class PullStateCommand : public Frontend::Runtime::PullStateCommand
    {
    public:
        using Frontend::Runtime::PullStateCommand::PullStateCommand;

        //--------------------------------------------------------------------
        // Command virtuals
        //--------------------------------------------------------------------
        //! Run any code required before the command execures on the main thread
        virtual void preamble(Frontend::Runtime &runtime) const override final
        {
            // Hand DMA buffer over to FeNN cores before starting DMA
            static_cast<RuntimeHW&>(runtime).setDMABufferAccessMode(Common::DMABuffer::AccessMode::FENN);
        }

        //! Run any code required after the command execures on the main thread
        virtual void postamble(Frontend::Runtime &runtime) const override final
        {
            // Once DMA is finished, hand DMA buffer back to CPU
            static_cast<RuntimeHW&>(runtime).setDMABufferAccessMode(Common::DMABuffer::AccessMode::CPU);
        }
    };

    //------------------------------------------------------------------------
    // Runtime virtuals
    //------------------------------------------------------------------------
    //! Factory method to create a run current kernel command object
    virtual std::unique_ptr<Command> createRunCurrentKernelCommand() const override final;

    //! Factory method to create a push state command object
    virtual std::unique_ptr<Command> createPushStateCommand(std::shared_ptr<const Frontend::State> state) const override final;

    // Factory method to create a pull state command object
    virtual std::unique_ptr<Command> createPullStateCommand(std::shared_ptr<const Frontend::State> state) const override final;


private:
    //------------------------------------------------------------------------
    // Runtime virtuals
    //------------------------------------------------------------------------
    virtual std::unique_ptr<Frontend::DeviceBase> createDevice(size_t deviceIndex) override final;

    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    void setDMABufferAccessMode(Common::DMABuffer::AccessMode mode);

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    size_t m_DMABufferSize;
    Common::DMABuffer m_ParentDMABuffer;
    Common::DeviceControl m_DeviceControl;
};
}