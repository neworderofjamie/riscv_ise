#include "backend/backend_fenn_hw.h"

// Standard C++ includes
#include <optional>
#include <stdexcept>

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/device.h"
#include "common/dma_buffer.h"
#include "common/dma_controller.h"

// Backend includes
#include "backend/memory_allocator.h"

//------------------------------------------------------------------------
// Anonymous namespace
//------------------------------------------------------------------------
namespace
{
class SimState : public StateBase
{
public:
    SimState() : m_DMABufferAllocator(m_DMABuffer)
    {}

    //------------------------------------------------------------------------
    // StateBase virtuals
    //------------------------------------------------------------------------
    virtual void setInstructions(const std::vector<uint32_t> &instructions) override final
    {
        m_Device.uploadCode(instructions);
    }

    virtual void run() override final
    {
        // Enable core
        m_Device.setEnabled(true);
        LOGD << "Running";
        
        // Wait until ready flag
        // **TODO** ready flag pointer
        m_Device.waitOnNonZero(0);
        LOGD << "Done";

        // Disable core
        m_Device.setEnabled(true);
    }

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
    Device m_Device;
    DMABuffer m_DMABuffer;
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
     URAMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state)
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
        m_DMABufferOffset = m_State->getDMABufferAllocator().allocate(getSizeBytes());
        setHostPointer(m_State->getDMABuffer().getData() + m_DMABufferOffset.value());

        // Allocate URAM
        setURAMPointer(m_State->getURAMAllocator().allocate(getSizeBytes()));
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
        auto *dmaController = m_State->getDevice().getDMAController();
        dmaController->startWrite(getURAMPointer(), m_State->getDMABuffer(),
                                  m_DMABufferOffset.value(), getSizeBytes());
        dmaController->waitForWriteComplete();
    }

    virtual void memsetHostPointer(int value) final override
    {
        // **YUCK** memset seems to do something that doens't play
        // nicely with mmap'd memory and causes bus error
        std::fill_n(getHostPointer(), getSizeBytes(), value);
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        throw std::runtime_error("URAM arrays cannot currently be pulled from device on FeNN");
    }

private:
    std::optional<size_t> m_DMABufferOffset;
    SimState *m_State;
};

//------------------------------------------------------------------------
// BRAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in BRAM. Host memory is allocated using standard
//! allocator and is transferred to FeNN using Device functionality
class BRAMArray : public BRAMArrayBase
{
public:
     BRAMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state)
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
        setBRAMPointer(m_State->getBRAMAllocator().allocate(getSizeBytes()));
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
        m_State->getDevice().memcpyDataToDevice(getBRAMPointer(), getHostPointer(),
                                                getSizeBytes());
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        m_State->getDevice().memcpyDataFromDevice(getHostPointer(), getBRAMPointer(), 
                                                  getSizeBytes());
    }

private:
     SimState *m_State;
};
}


//----------------------------------------------------------------------------
// BackendFeNNHW
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNNHW::createURAMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                           StateBase *state) const
{
    return std::make_unique<::URAMArray>(type, count, static_cast<SimState*>(state));
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNNHW::createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                           StateBase *state) const
{
    return std::make_unique<::BRAMArray>(type, count, static_cast<SimState*>(state));
}
//------------------------------------------------------------------------
std::unique_ptr<IFieldArray> BackendFeNNHW::createFieldArray(const Model &model, StateBase *state) const
{
    return std::make_unique<::BRAMFieldArray<BRAMArray>>(GeNN::Type::Uint32, model.getNumFields(), static_cast<SimState*>(state));
}
//------------------------------------------------------------------------
std::unique_ptr<StateBase> BackendFeNNHW::createState() const
{
    return std::make_unique<SimState>();
}