#include "backend/backend_fenn_sim.h"

// Standard C++ includes
#include <stdexcept>

// Plog includes
#include <plog/Log.h>

// Common includes
#include "common/utils.h"

// ISE includes
#include "ise/dma_controller_sim.h"
#include "ise/riscv.h"
#include "ise/vector_processor.h"

// Backend includes
#include "backend/memory_allocator.h"


//------------------------------------------------------------------------
// Anonymous namespace
//------------------------------------------------------------------------
namespace
{
// Forward declarations
class SimState;

//------------------------------------------------------------------------
// URAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in URAM. Host memory lives in 
//! (uncached) DMA buffer and is transferred to FeNN using DMA controller
class URAMArray : public URAMArrayBase
{
public:
    URAMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state);
    virtual ~URAMArray();

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override;

    //! Copy entire array from device
    virtual void pullFromDevice() final override;

private:
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
    BRAMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state);
    virtual ~BRAMArray();

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override;

    //! Copy entire array from device
    virtual void pullFromDevice() final override;

private:
    SimState *m_State;
};

//------------------------------------------------------------------------
// LLMArray
//------------------------------------------------------------------------
//! Class for managing arrays in lane-local memory.
class LLMArray : public LLMArrayBase
{
public:
    LLMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state);
    virtual ~LLMArray();

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override;

    //! Copy entire array from device
    virtual void pullFromDevice() final override;

private:
    SimState *m_State;
};

//------------------------------------------------------------------------
// DRAMArray
//------------------------------------------------------------------------
//! Class for managing arrays in DRAM. 
class DRAMArray : public DRAMArrayBase
{
public:
    DRAMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state);
    virtual ~DRAMArray();

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override;

    //! Copy entire array from device
    virtual void pullFromDevice() final override;

private:
    SimState *m_State;
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
                 SimState *state);
    virtual ~URAMLLMArray();

    //------------------------------------------------------------------------
    // ArrayBase virtuals
    //------------------------------------------------------------------------
    //! Copy entire array to device
    virtual void pushToDevice() final override;

    //! Copy entire array from device
    virtual void pullFromDevice() final override;

private:
    SimState *m_State;
};

//----------------------------------------------------------------------------
// SimState
//----------------------------------------------------------------------------
class SimState : public StateBase
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

    virtual void run() override final
    {
        m_RISCV.setPC(0);
        m_RISCV.run();
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
};


//------------------------------------------------------------------------
// URAMArray
//------------------------------------------------------------------------
URAMArray::URAMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state)
:   URAMArrayBase(type, count), m_State(state)
{
    // Allocate if count is specified
    if(count > 0) {
        // Allocate memory for host pointer
        setHostPointer(new uint8_t[getSizeBytes()]);

        // Allocate URAM
        setURAMPointer(m_State->getURAMAllocator().allocate(getSizeBytes()));
    }
}
//------------------------------------------------------------------------
URAMArray::~URAMArray()
{
    if(getCount() > 0) {
        delete [] getHostPointer();
        setHostPointer(nullptr);
        setURAMPointer(std::nullopt);
    }
}
//------------------------------------------------------------------------
void URAMArray::pushToDevice()
{
    // Copy correct number of int16_t from host pointer to vector data memory
    auto &vectorDataMemory = m_State->getRISCV().getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory();
    std::copy_n(getHostPointer<int16_t>(), getCount(), 
                vectorDataMemory.getData() + (getURAMPointer() / 2));
}
//------------------------------------------------------------------------
void URAMArray::pullFromDevice()
{
    // Copy correct number of int16_t from vector data memory to host pointer
    const auto &vectorDataMemory = m_State->getRISCV().getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory();
    std::copy_n(vectorDataMemory.getData() + (getURAMPointer() / 2), getCount(), 
                getHostPointer<int16_t>());
}
//------------------------------------------------------------------------
// BRAMArray
//------------------------------------------------------------------------
BRAMArray::BRAMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state)
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
    // Copy correct number of int16_t from host pointer to vector data memory
    auto &scalarDataMemory = m_State->getRISCV().getScalarDataMemory();
    std::copy_n(getHostPointer<uint8_t>(), getSizeBytes(), 
                scalarDataMemory.getData() + getBRAMPointer());
}
//------------------------------------------------------------------------
void BRAMArray::pullFromDevice()
{
    // Copy correct number of int16_t from vector data memory to host pointer
    const auto &scalarDataMemory = m_State->getRISCV().getScalarDataMemory();
    std::copy_n(scalarDataMemory.getData() + getBRAMPointer(), getSizeBytes(), 
                getHostPointer<uint8_t>());
}

//------------------------------------------------------------------------
// LLMArray
//------------------------------------------------------------------------
LLMArray::LLMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state)
:   LLMArrayBase(type, count), m_State(state)
{
    // Allocate if count is specified
    if(count > 0) {
        // Allocate memory for host pointer
        setHostPointer(nullptr);

        // Allocate LLM
        setLLMPointer(m_State->getLLMAllocator().allocate(getSizeBytes()));
    }
}
//------------------------------------------------------------------------
LLMArray::~LLMArray()
{
    if(getCount() > 0) {
        delete [] getHostPointer();
        setHostPointer(nullptr);
        setLLMPointer(std::nullopt);
    }
}
//------------------------------------------------------------------------
void LLMArray::pushToDevice()
{
    LOGW << "Copying LLM buffers is implemented in simulation for convenience but is not possible on device";
    const size_t numRows = ceilDivide(getCount(), 32);
    for(size_t l = 0; l < 32; l++) {
        auto &laneLocalMemory = m_State->getRISCV().getCoprocessor<VectorProcessor>(vectorQuadrant)->getLaneLocalMemory(l);    
        int16_t *llmPointer = laneLocalMemory.getData() + (getLLMPointer() / 2);
        for(size_t r = 0; r < numRows; r++) {
            *llmPointer++ = getHostPointer<int16_t>()[(r * 32) + l];
        }
    }
}
//------------------------------------------------------------------------
void LLMArray::pullFromDevice()
{
    LOGW << "Copying LLM buffers is implemented in simulation for convenience but is not possible on device";
        
    const size_t numRows = ceilDivide(getCount(), 32);
    for(size_t l = 0; l < 32; l++) {
        const auto &laneLocalMemory = m_State->getRISCV().getCoprocessor<VectorProcessor>(vectorQuadrant)->getLaneLocalMemory(l);    
        const int16_t *llmPointer = laneLocalMemory.getData() + (getLLMPointer() / 2);
        for(size_t r = 0; r < numRows; r++) {
            getHostPointer<int16_t>()[(r * 32) + l] = *llmPointer++;
        }
    }
}

//------------------------------------------------------------------------
// DRAMArray
//------------------------------------------------------------------------
DRAMArray::DRAMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state)
:   DRAMArrayBase(type, count), m_State(state)
{
    // Allocate if count is specified
    if(count > 0) {
        // Allocate block of DMA buffer
        const size_t offset = m_State->getDMABufferAllocator().allocate(getSizeBytes());

        // Add to virtual address of DMA buffer data to get host pointer
        setHostPointer(m_State->getDMAController()->getData() + offset);

        // Use directly as device pointer
        // **NOTE** simulated DMA controller has a physical address of 0
        setDRAMPointer(offset);
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
                           SimState *state)
:   URAMLLMArrayBase(type, uramCount, llmCount), m_State(state)
{
    if(uramCount > 0) {
        // Allocate memory for host pointer
        setHostPointer(new uint8_t[getSizeBytes()]);

        // Allocate URAM
        setURAMPointer(m_State->getURAMAllocator().allocate(getSizeBytes()));
    }

    // Allocate LLM
    if(llmCount > 0) {
        setLLMPointer(m_State->getLLMAllocator().allocate(getLLMSizeBytes()));
    }
}
//------------------------------------------------------------------------
URAMLLMArray::~URAMLLMArray()
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
void URAMLLMArray::pushToDevice()
{
    // Copy correct number of int16_t from host pointer to vector data memory
    auto &vectorDataMemory = m_State->getRISCV().getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory();
    std::copy_n(getHostPointer<int16_t>(), getCount(), 
                vectorDataMemory.getData() + (getURAMPointer() / 2));
}
//------------------------------------------------------------------------
void URAMLLMArray::pullFromDevice()
{
    // Copy correct number of int16_t from vector data memory to host pointer
    const auto &vectorDataMemory = m_State->getRISCV().getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory();
    std::copy_n(vectorDataMemory.getData() + (getURAMPointer() / 2), getCount(), 
                getHostPointer<int16_t>());
}
}

//----------------------------------------------------------------------------
// BackendFeNNSim
//------------------------------------------------------------------------
BackendFeNNSim::BackendFeNNSim(bool useDRAMForWeights, bool keepParamsInRegisters, 
                               RoundingMode neuronUpdateRoundingMode, size_t dmaBufferSize)
:   BackendFeNN(useDRAMForWeights, keepParamsInRegisters, neuronUpdateRoundingMode), 
    m_DMABufferSize(dmaBufferSize)
{
}
//------------------------------------------------------------------------
std::unique_ptr<StateBase> BackendFeNNSim::createState() const
{
    return std::make_unique<SimState>(m_DMABufferSize);
}