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
    State() : m_DMABufferAllocator(m_DMABuffer)
    {}

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    const auto &getDevice() const{ return m_Device; }
    auto &getDevice(){ return m_Device; }

    const auto &getBRAMAllocator() const{ return m_BRAMAllocator; }
    auto &getBRAMAllocator(){ return m_BRAMAllocator; }

    const auto &getURAMAllocator() const{ return m_URAMAllocator; }
    auto &getURAMAllocator(){ return m_URAMAllocator; }

    const auto &getDMABufferAllocator() const{ return m_DMABufferAllocator; }
    auto &getDMABufferAllocator(){ return m_DMABufferAllocator; }

    const auto &getDMABuffer() const{ return m_DMABuffer; }
    auto &getDMABuffer(){ return m_DMABuffer; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    Device m_Device;
    DMABuffer m_DMABuffer;
    BRAMAllocator m_BRAMAllocator;
    URAMAllocator m_URAMAllocator;
    DMABufferAllocator m_DMABufferAllocator;
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
        // Start DMA write and wait for completion
        auto *dmaController = m_State.get().getDevice().getDMAController();
        dmaController->startWrite(getURAMPointer(), m_State.get().getDMABuffer(),
                                  m_DMABufferOffset.value(), getSizeBytes());
        dmaController->waitForWriteComplete();
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