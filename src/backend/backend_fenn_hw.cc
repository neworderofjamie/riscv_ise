#include "backend/backend_fenn_hw.h"

// Standard C++ includes
#include <optional>
#include <stdexcept>

// Common includes
#include "common/device.h"
#include "common/dma_buffer.h"
#include "common/dma_controller.h"

// Compiler includes
#include "compiler/memory_allocator.h"

//------------------------------------------------------------------------
// Anonymous namespace
//------------------------------------------------------------------------
namespace
{
class State : public StateBase
{
public:
    State(Device &device, BRAMAllocator &bramAllocator, URAMAllocator &uramAllocator, 
          DMABufferAllocator &dmaBufferAllocator, DMABuffer &dmaBuffer, DMAController &dmaController)
    :   m_Device(device), m_BRAMAllocator(bramAllocator), m_URAMAllocator(uramAllocator), 
        m_DMABufferAllocator(dmaBufferAllocator), m_DMABuffer(dmaBuffer), m_DMAController(dmaController)
    {}

    const auto &getDevice() const{ return m_Device.get(); }
    auto &getDevice(){ return m_Device.get(); }

    const auto &getBRAMAllocator() const{ return m_BRAMAllocator.get(); }
    auto &getBRAMAllocator(){ return m_BRAMAllocator.get(); }

    const auto &getURAMAllocator() const{ return m_URAMAllocator.get(); }
    auto &getURAMAllocator(){ return m_URAMAllocator.get(); }

    const auto &getDMABufferAllocator() const{ return m_DMABufferAllocator.get(); }
    auto &getDMABufferAllocator(){ return m_DMABufferAllocator.get(); }

    const auto &getDMABuffer() const{ return m_DMABuffer.get(); }
    auto &getDMABuffer(){ return m_DMABuffer.get(); }

    const auto &getDMAController() const{ return m_DMAController.get(); }
    auto &getDMAController(){ return m_DMAController.get(); }

private:
    std::reference_wrapper<Device> m_Device;
    std::reference_wrapper<BRAMAllocator> m_BRAMAllocator;
    std::reference_wrapper<URAMAllocator> m_URAMAllocator;
    std::reference_wrapper<DMABufferAllocator> m_DMABufferAllocator;
    std::reference_wrapper<DMABuffer> m_DMABuffer;
    std::reference_wrapper<DMAController> m_DMAController;
};

//------------------------------------------------------------------------
// URAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in URAM. Host memory lives in 
//! (uncached) DMA buffer and is transferred to FeNN using DMA controller
class URAMArray : public URAMArrayBase
{
public:
     URAMArray(const GeNN::Type::ResolvedType &type, size_t count, State &state)
    :   URAMArrayBase(type, count), m_State(state)
    {
        // Allocate if count is specified
        if(count > 0) {
            allocate(count);
        }
    }

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Allocate array
    virtual void allocate(size_t count) final override
    {
         // Set count
        setCount(count);

        // Allocate block of DMA buffer and set host pointer
        m_DMABufferOffset = m_State.get().getDMABufferAllocator().allocate(getSizeBytes());
        setHostPointer(m_State.get().getDMABuffer().getData() + m_DMABufferOffset.value());

        // Allocate URAM
        setURAMPointer(m_State.get().getURAMAllocator().allocate(getSizeBytes()));
    }

    //! Free array
    virtual void free() final override
    {
        setHostPointer(nullptr);
        setURAMPointer(std::nullopt);
        m_DMABufferOffset.reset();
        setCount(0);
    }

    //! Copy entire array to device
    virtual void pushToDevice() final override
    {
        // **TODO** use DMA controller
        
        // Start DMA write and wait for completion
        m_State.get().getDMAController().startWrite(getURAMPointer(), m_State.get().getDMABuffer(),
                                                    m_DMABufferOffset.value(), getSizeBytes());
        m_State.get().getDMAController().waitForWriteComplete();
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        throw std::runtime_error("URAM arrays cannot currently be pulled from device on FeNN");
    }

private:
    std::optional<size_t> m_DMABufferOffset;
    std::reference_wrapper<State> m_State;
};

//------------------------------------------------------------------------
// BRAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in BRAM. Host memory is allocated using standard
//! allocator and is transferred to FeNN using Device functionality
class BRAMArray : public BRAMArrayBase
{
public:
     BRAMArray(const GeNN::Type::ResolvedType &type, size_t count, State &state)
    :   BRAMArrayBase(type, count), m_State(state)
    {
        // Allocate if count is specified
        if(count > 0) {
            allocate(count);
        }
    }

    virtual ~BRAMArray()
    {
        if(getCount() > 0) {
            free();
        }
    }

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Allocate array
    virtual void allocate(size_t count) final override
    {
         // Set count
        setCount(count);

        // Allocate memory for host pointer
        setHostPointer(new uint8_t[getSizeBytes()]);

        // Allocate BRAM
        setBRAMPointer(m_State.get().getBRAMAllocator().allocate(getSizeBytes()));
    }

    //! Free array
    virtual void free() final override
    {
        delete [] getHostPointer();
        setHostPointer(nullptr);
        setBRAMPointer(std::nullopt);
        setCount(0);
    }

    //! Copy entire array to device
    virtual void pushToDevice() final override
    {
        m_State.get().getDevice().memcpyDataToDevice(getBRAMPointer(), getHostPointer(),
                                                     getSizeBytes());
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        m_State.get().getDevice().memcpyDataFromDevice(getHostPointer(), getBRAMPointer(), 
                                                       getSizeBytes());
    }

private:
     std::reference_wrapper<State> m_State;
};
}