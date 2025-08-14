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
// Forward declarations
class HWState;

//------------------------------------------------------------------------
// URAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in URAM. Host memory lives in 
//! (uncached) DMA buffer and is transferred to FeNN using DMA controller
class URAMArray : public URAMArrayBase
{
public:
    URAMArray(const GeNN::Type::ResolvedType &type, size_t count, HWState *state);

    virtual ~URAMArray();

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Allocate array
    virtual void allocate(size_t count) final override;

    //! Free array
    virtual void free() final override;

    //! Copy entire array to device
    virtual void pushToDevice() final override;

    virtual void memsetHostPointer(int value) final override;

    //! Copy entire array from device
    virtual void pullFromDevice() final override;

private:
    std::optional<size_t> m_DMABufferOffset;
    HWState *m_State;
};

//------------------------------------------------------------------------
// BRAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in BRAM. Host memory is allocated using standard
//! allocator and is transferred to FeNN using Device functionality
class BRAMArray : public BRAMArrayBase
{
public:
    BRAMArray(const GeNN::Type::ResolvedType &type, size_t count, HWState *state);

    virtual ~BRAMArray();

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Allocate array
    virtual void allocate(size_t count) final override;

    //! Free array
    virtual void free() final override;

    //! Copy entire array to device
    virtual void pushToDevice() final override;

    //! Copy entire array from device
    virtual void pullFromDevice() final override;

private:
     HWState *m_State;
};

//------------------------------------------------------------------------
// LLMArray
//------------------------------------------------------------------------
//! Class for managing arrays in lane-local memory.
class LLMArray : public LLMArrayBase
{
public:
    LLMArray(const GeNN::Type::ResolvedType &type, size_t count, HWState *state);

    virtual ~LLMArray();

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Allocate array
    virtual void allocate(size_t count) final override;

    //! Free array
    virtual void free() final override;

    //! Copy entire array to device
    virtual void pushToDevice() final override;

    //! Copy entire array from device
    virtual void pullFromDevice() final override;

private:
    HWState *m_State;
};

//------------------------------------------------------------------------
// DRAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in DRAM. 
class DRAMArray : public DRAMArrayBase
{
public:
    DRAMArray(const GeNN::Type::ResolvedType &type, size_t count, HWState *state);

    virtual ~DRAMArray();

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Allocate array
    virtual void allocate(size_t count) final override;

    //! Free array
    virtual void free() final override;

    //! Copy entire array to device
    virtual void pushToDevice() final override;

    //! Copy entire array from device
    virtual void pullFromDevice() final override;

private:
    HWState *m_State;
};

//----------------------------------------------------------------------------
// HWState
//----------------------------------------------------------------------------
class HWState : public StateBase
{
public:
    HWState() 
    :   m_DMABufferAllocator(m_DMABuffer.getSize())
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
        m_Device.waitOnNonZero(0);
        LOGD << "Done";

        // Disable core
        m_Device.setEnabled(false);
    }

    std::unique_ptr<ArrayBase> createURAMArray(const GeNN::Type::ResolvedType &type, size_t count) final override
    {
        return std::make_unique<::URAMArray>(type, count, this);
    }
    
    std::unique_ptr<ArrayBase> createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count) final override
    {
        return std::make_unique<::BRAMArray>(type, count, this);
    }
    
    std::unique_ptr<ArrayBase> createLLMArray(const GeNN::Type::ResolvedType &type, size_t count) final override
    {
        return std::make_unique<::LLMArray>(type, count, this);
    }
    
    std::unique_ptr<ArrayBase> createDRAMArray(const GeNN::Type::ResolvedType &type, size_t count) final override
    {
        return std::make_unique<::DRAMArray>(type, count, this);
    }

    std::unique_ptr<IFieldArray> createFieldArray(const Model &model) final override
    {
        return std::make_unique<::BRAMFieldArray<BRAMArray>>(GeNN::Type::Uint8, model.getNumFieldBytes(), this);
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
URAMArray::URAMArray(const GeNN::Type::ResolvedType &type, size_t count, HWState *state)
:   URAMArrayBase(type, count), m_State(state)
{
    // Allocate if count is specified
    if(count > 0) {
        allocate(count);
    }
}

URAMArray::~URAMArray()
{
    if(getCount() > 0) {
        free();
    }
}

//------------------------------------------------------------------------
// ArrayBase virtuals
//------------------------------------------------------------------------
//! Allocate array
void URAMArray::allocate(size_t count)
{
        // Set count
    setCount(count);

    // Allocate block of DMA buffer and set host pointer
    m_DMABufferOffset = m_State->getDMABufferAllocator().allocate(getSizeBytes());
    setHostPointer(m_State->getDMABuffer().getData() + m_DMABufferOffset.value());

    // Allocate URAM
    setURAMPointer(m_State->getURAMAllocator().allocate(getSizeBytes()));
}
//------------------------------------------------------------------------
void URAMArray::free()
{
    setHostPointer(nullptr);
    setURAMPointer(std::nullopt);
    m_DMABufferOffset.reset();
    setCount(0);
}
//------------------------------------------------------------------------
void URAMArray::pushToDevice()
{
    // Start DMA write and wait for completion
    auto *dmaController = m_State->getDevice().getDMAController();
    dmaController->startWrite(getURAMPointer(), m_State->getDMABuffer(),
                                m_DMABufferOffset.value(), getSizeBytes());
    dmaController->waitForWriteComplete();
}
//------------------------------------------------------------------------
void URAMArray::memsetHostPointer(int value)
{
    // **YUCK** memset seems to do something that doens't play
    // nicely with mmap'd memory and causes bus error
    for (size_t i = 0; i < getSizeBytes(); i++) {
        getHostPointer()[i] = value;
    }
}
//------------------------------------------------------------------------
void URAMArray::pullFromDevice()
{
    // Start DMA read and wait for completion
    auto *dmaController = m_State->getDevice().getDMAController();
    dmaController->startRead(m_State->getDMABuffer(), m_DMABufferOffset.value(), 
                             getURAMPointer(), getSizeBytes());
    dmaController->waitForReadComplete();
}


//------------------------------------------------------------------------
// BRAMArray
//------------------------------------------------------------------------
BRAMArray::BRAMArray(const GeNN::Type::ResolvedType &type, size_t count, HWState *state)
:   BRAMArrayBase(type, count), m_State(state)
{
    // Allocate if count is specified
    if(count > 0) {
        allocate(count);
    }
}
//------------------------------------------------------------------------
BRAMArray::~BRAMArray()
{
    if(getCount() > 0) {
        free();
    }
}
//------------------------------------------------------------------------
void BRAMArray::allocate(size_t count)
{
    // Set count
    setCount(count);

    // Allocate memory for host pointer
    setHostPointer(new uint8_t[getSizeBytes()]);

    // Allocate BRAM
    setBRAMPointer(m_State->getBRAMAllocator().allocate(getSizeBytes()));
}
//------------------------------------------------------------------------
void BRAMArray::free()
{
    delete [] getHostPointer();
    setHostPointer(nullptr);
    setBRAMPointer(std::nullopt);
    setCount(0);
}
//------------------------------------------------------------------------
void BRAMArray::pushToDevice()
{
    m_State->getDevice().memcpyDataToDevice(getBRAMPointer(), getHostPointer(),
                                            getSizeBytes());
}
//------------------------------------------------------------------------
void BRAMArray::pullFromDevice()
{
    m_State->getDevice().memcpyDataFromDevice(getHostPointer(), getBRAMPointer(), 
                                                getSizeBytes());
}

//------------------------------------------------------------------------
// LLMArray
//------------------------------------------------------------------------
LLMArray::LLMArray(const GeNN::Type::ResolvedType &type, size_t count, HWState *state)
:   LLMArrayBase(type, count), m_State(state)
{
    // Allocate if count is specified
    if(count > 0) {
        allocate(count);
    }
}
//------------------------------------------------------------------------
LLMArray::~LLMArray()
{
    if(getCount() > 0) {
        free();
    }
}
//------------------------------------------------------------------------
void LLMArray::allocate(size_t count)
{
    // Set count
    setCount(count);

    // Don't allocate memory for host pointer
    setHostPointer(nullptr);

    // Allocate LLM
    setLLMPointer(m_State->getLLMAllocator().allocate(getSizeBytes()));
}
//------------------------------------------------------------------------
void LLMArray::free()
{
    setLLMPointer(std::nullopt);
    setCount(0);
}
//------------------------------------------------------------------------
void LLMArray::pushToDevice()
{
    throw std::runtime_error("LLM arrays cannot be pushed to device on FeNN");
}
//------------------------------------------------------------------------
void LLMArray::pullFromDevice()
{
    throw std::runtime_error("LLM arrays cannot be pulled from device on FeNN");
}


//------------------------------------------------------------------------
// DRAMArray
//------------------------------------------------------------------------
DRAMArray::DRAMArray(const GeNN::Type::ResolvedType &type, size_t count, HWState *state)
:   DRAMArrayBase(type, count), m_State(state)
{
    // Allocate if count is specified
    if(count > 0) {
        allocate(count);
    }
}
//------------------------------------------------------------------------
DRAMArray::~DRAMArray()
{
    if(getCount() > 0) {
        free();
    }
}
//------------------------------------------------------------------------
void DRAMArray::allocate(size_t count)
{
    // Set count
    setCount(count);
        
    // Allocate block of DMA buffer
    const size_t offset = m_State->getDMABufferAllocator().allocate(getSizeBytes());

    // Add to virtual address of DMA buffer data to get host pointer
    setHostPointer(m_State->getDMABuffer().getData() + offset);

    // Add to physical address of DMA buffer to get device pointer
    setDRAMPointer(m_State->getDMABuffer().getPhysicalAddress() + offset);
}
//------------------------------------------------------------------------
void DRAMArray::free()
{
    // **NOTE** no memory is owned by array so just invalidate
    setHostPointer(nullptr);
    setDRAMPointer(std::nullopt);
    setCount(0);
}
//------------------------------------------------------------------------
void DRAMArray::pushToDevice()
{
}
//------------------------------------------------------------------------
void DRAMArray::pullFromDevice()
{
}
}


//----------------------------------------------------------------------------
// BackendFeNNHW
//------------------------------------------------------------------------
std::unique_ptr<StateBase> BackendFeNNHW::createState() const
{
    return std::make_unique<HWState>();
}