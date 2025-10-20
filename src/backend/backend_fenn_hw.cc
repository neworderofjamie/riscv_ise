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
    //! Copy entire array to device
    virtual void pushToDevice() final override;

    //! Copy entire array from device
    virtual void pullFromDevice() final override;

private:
    HWState *m_State;
};

//------------------------------------------------------------------------
// URAMLLMArray
//------------------------------------------------------------------------
//! class for arrays which are allocated in URAM but also have a delayed input in LLM
//! Typically used for implementing neuron variables with dendritically-delayed input 
class URAMLLMArray : public URAMLLMArrayBase
{
public:
    URAMLLMArray(const GeNN::Type::ResolvedType &type, size_t uramCount, size_t llmCount,
                 HWState *state);
    virtual ~URAMLLMArray();

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override;
    
    virtual void memsetHostPointer(int value) final override;

    //! Copy entire array from device
    virtual void pullFromDevice() final override;

private:
    std::optional<size_t> m_DMABufferOffset;
    HWState *m_State;
};

//----------------------------------------------------------------------------
// HWState
//----------------------------------------------------------------------------
class HWState : public StateBase
{
public:
    HWState(unsigned int core, unsigned int numCores) 
    :   m_Device(0x80000000 + (core * 0x01000000)), 
        m_DMABuffer(getParentDMABuffer(), 
                    (numCores == 1) ? getParentDMABuffer().getPhysicalAddress() : (0x40000000 + (core * 0x10000000)),
                    (numCores == 1) ? (getParentDMABuffer().getPhysicalAddress() + getParentDMABuffer().getSize()) : (0x50000000 + (core * 0x10000000))),
        m_DMABufferAllocator(m_DMABuffer.getSize())
    {}

    //------------------------------------------------------------------------
    // StateBase virtuals
    //------------------------------------------------------------------------
    virtual void setInstructions(const std::vector<uint32_t> &instructions) override final
    {
        m_Device.uploadCode(instructions);
    }

    virtual void startRun() override final
    {
        // Enable core
        m_Device.setEnabled(true);
        LOGD << "Running";
        
    }

    virtual void waitRun() override final
    {
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

    virtual std::unique_ptr<ArrayBase> createURAMLLMArray(const GeNN::Type::ResolvedType &type,
                                                          size_t uramCount, size_t llmCount) final override
    {
        return std::make_unique<::URAMLLMArray>(type, uramCount, llmCount, this);
    }


    std::unique_ptr<IFieldArray> createFieldArray(const Model &model) final override
    {
        return std::make_unique<::BRAMFieldArray<BRAMArray>>(GeNN::Type::Uint8, model.getNumFieldBytes(), this);
    }

    virtual std::optional<unsigned int> getSOCPower() const final override
    {
        return m_Device.getSOCPower();
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
    // Static API
    //------------------------------------------------------------------------
    static DMABuffer &getParentDMABuffer()
    {
        static std::unique_ptr<DMABuffer> parentDMABuffer = std::make_unique<DMABuffer>();
        return *parentDMABuffer.get();
    }

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
        // Allocate block of DMA buffer and set host pointer
        m_DMABufferOffset = m_State->getDMABufferAllocator().allocate(getSizeBytes());
        setHostPointer(m_State->getDMABuffer().getData() + m_DMABufferOffset.value());

        // Allocate URAM
        setURAMPointer(m_State->getURAMAllocator().allocate(getSizeBytes()));
    }
}
//------------------------------------------------------------------------
URAMArray::~URAMArray()
{
    if(getCount() > 0) {
        setHostPointer(nullptr);
        setURAMPointer(std::nullopt);
        m_DMABufferOffset.reset();
    }
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
        // Allocate memory for host pointer
        setHostPointer(new uint8_t[getSizeBytes()]);

        // Allocate BRAM
        setBRAMPointer(m_State->getBRAMAllocator().allocate(getSizeBytes()));
    }
}
//------------------------------------------------------------------------
BRAMArray::~BRAMArray()
{
    if(getCount() > 0) {
        delete [] getHostPointer();
        setHostPointer(nullptr);
        setBRAMPointer(std::nullopt);
    }
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
        // Don't allocate memory for host pointer
        setHostPointer(nullptr);

        // Allocate LLM
        setLLMPointer(m_State->getLLMAllocator().allocate(getSizeBytes()));
    }
}
//------------------------------------------------------------------------
LLMArray::~LLMArray()
{
    if(getCount() > 0) {
        setLLMPointer(std::nullopt);
    }
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
        // Allocate block of DMA buffer
        const size_t offset = m_State->getDMABufferAllocator().allocate(getSizeBytes());

        // Add to virtual address of DMA buffer data to get host pointer
        setHostPointer(m_State->getDMABuffer().getData() + offset);

        // Add to physical address of DMA buffer to get device pointer
        setDRAMPointer(m_State->getDMABuffer().getPhysicalAddress() + offset);
    }
}
//------------------------------------------------------------------------
DRAMArray::~DRAMArray()
{
    if(getCount() > 0) {
        // **NOTE** no memory is owned by array so just invalidate
        setHostPointer(nullptr);
        setDRAMPointer(std::nullopt);
    }
}
//------------------------------------------------------------------------
void DRAMArray::pushToDevice()
{
}
//------------------------------------------------------------------------
void DRAMArray::pullFromDevice()
{
}

//------------------------------------------------------------------------
// URAMLLMArray
//------------------------------------------------------------------------
URAMLLMArray::URAMLLMArray(const GeNN::Type::ResolvedType &type, size_t uramCount, size_t llmCount,
                           HWState *state)
:   URAMLLMArrayBase(type, uramCount, llmCount), m_State(state)
{
    if(uramCount > 0) {
        // Allocate block of DMA buffer and set host pointer
        m_DMABufferOffset = m_State->getDMABufferAllocator().allocate(getSizeBytes());
        setHostPointer(m_State->getDMABuffer().getData() + m_DMABufferOffset.value());

        // Allocate URAM
        setURAMPointer(m_State->getURAMAllocator().allocate(getSizeBytes()));
    }

    // Allocate LLM
    if(llmCount > 0) {
        setLLMPointer(m_State->getLLMAllocator().allocate(getSizeBytes()));
    }
}
//------------------------------------------------------------------------
URAMLLMArray::~URAMLLMArray()
{
    if(getCount() > 0) {
        setHostPointer(nullptr);
        setURAMPointer(std::nullopt);
        m_DMABufferOffset.reset();
    }

    if(getLLMCount() > 0) {
        setLLMPointer(std::nullopt);
    }
}
//------------------------------------------------------------------------
void URAMLLMArray::pushToDevice()
{
    // Start DMA write and wait for completion
    auto *dmaController = m_State->getDevice().getDMAController();
    dmaController->startWrite(getURAMPointer(), m_State->getDMABuffer(),
                                m_DMABufferOffset.value(), getSizeBytes());
    dmaController->waitForWriteComplete();
}
//------------------------------------------------------------------------
void URAMLLMArray::memsetHostPointer(int value)
{
    // **YUCK** memset seems to do something that doens't play
    // nicely with mmap'd memory and causes bus error
    for (size_t i = 0; i < getSizeBytes(); i++) {
        getHostPointer()[i] = value;
    }
}
//------------------------------------------------------------------------
void URAMLLMArray::pullFromDevice()
{
    // Start DMA read and wait for completion
    auto *dmaController = m_State->getDevice().getDMAController();
    dmaController->startRead(m_State->getDMABuffer(), m_DMABufferOffset.value(), 
                             getURAMPointer(), getSizeBytes());
    dmaController->waitForReadComplete();
}
}

//----------------------------------------------------------------------------
// BackendFeNNHW
//------------------------------------------------------------------------
BackendFeNNHW::BackendFeNNHW(bool useDRAMForWeights, bool keepParamsInRegisters, 
                             RoundingMode neuronUpdateRoundingMode, 
                             unsigned int core, unsigned int numCores)
:   BackendFeNN(useDRAMForWeights, keepParamsInRegisters, neuronUpdateRoundingMode), 
    m_Core(core), m_NumCores(numCores)
{
}
//------------------------------------------------------------------------
std::unique_ptr<StateBase> BackendFeNNHW::createState() const
{
    return std::make_unique<HWState>(m_Core, m_NumCores);
}
