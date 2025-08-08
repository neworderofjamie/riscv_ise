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
class SimState : public StateBase
{
public:
    SimState(size_t dmaBufferSize = 512 * 1024)
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

    virtual ~URAMArray()
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

        // Allocate URAM
        setURAMPointer(m_State->getURAMAllocator().allocate(getSizeBytes()));
    }

    //! Free array
    virtual void free() final override
    {
        delete [] getHostPointer();
        setHostPointer(nullptr);
        setURAMPointer(std::nullopt);
        setCount(0);
    }

    //! Copy entire array to device
    virtual void pushToDevice() final override
    {
        // Copy correct number of int16_t from host pointer to vector data memory
        auto &vectorDataMemory = m_State->getRISCV().getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory();
        std::copy_n(getHostPointer<int16_t>(), getCount(), 
                    vectorDataMemory.getData() + (getURAMPointer() / 2));
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        LOGW << "Copying URAM buffers is implemented in simulation for convenience but currently doens't work on device";
        
        // Copy correct number of int16_t from vector data memory to host pointer
        const auto &vectorDataMemory = m_State->getRISCV().getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory();
        std::copy_n(vectorDataMemory.getData() + (getURAMPointer() / 2), getCount(), 
                    getHostPointer<int16_t>());
    }

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
        // Copy correct number of int16_t from host pointer to vector data memory
        auto &scalarDataMemory = m_State->getRISCV().getScalarDataMemory();
        std::copy_n(getHostPointer<uint8_t>(), getSizeBytes(), 
                    scalarDataMemory.getData() + getBRAMPointer());
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
        // Copy correct number of int16_t from vector data memory to host pointer
        const auto &scalarDataMemory = m_State->getRISCV().getScalarDataMemory();
        std::copy_n(scalarDataMemory.getData() + getBRAMPointer(), getSizeBytes(), 
                    getHostPointer<uint8_t>());
    }

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
    LLMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state)
    :   LLMArrayBase(type, count), m_State(state)
    {
        // Allocate if count is specified
        if(count > 0) {
            allocate(count);
        }
    }

    virtual ~LLMArray()
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
        setHostPointer(nullptr);

        // Allocate LLM
        setLLMPointer(m_State->getLLMAllocator().allocate(getSizeBytes()));
    }

    //! Free array
    virtual void free() final override
    {
        delete [] getHostPointer();
        setHostPointer(nullptr);
        setLLMPointer(std::nullopt);
        setCount(0);
    }

    //! Copy entire array to device
    virtual void pushToDevice() final override
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

    //! Copy entire array from device
    virtual void pullFromDevice() final override
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
    DRAMArray(const GeNN::Type::ResolvedType &type, size_t count, SimState *state)
    :   DRAMArrayBase(type, count), m_State(state)
    {
        // Allocate if count is specified
        if(count > 0) {
            allocate(count);
        }
    }

    virtual ~DRAMArray()
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

        // Allocate block of DMA buffer
        const size_t offset = m_State->getDMABufferAllocator().allocate(getSizeBytes());

        // Add to virtual address of DMA buffer data to get host pointer
        setHostPointer(m_State->getDMAController()->getData() + offset);

        // Use directly as device pointer
        // **NOTE** simulated DMA controller has a physical address of 0
        setDRAMPointer(offset);
    }

    //! Free array
    virtual void free() final override
    {
        // **NOTE** no memory is owned by array so just invalidate
        setHostPointer(nullptr);
        setDRAMPointer(std::nullopt);
        setCount(0);
    }

    //! Copy entire array to device
    virtual void pushToDevice() final override
    {
    }

    //! Copy entire array from device
    virtual void pullFromDevice() final override
    {
    }

private:
    SimState *m_State;
};
}

//----------------------------------------------------------------------------
// BackendFeNNSim
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNNSim::createURAMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                           StateBase *state) const
{
    return std::make_unique<::URAMArray>(type, count, static_cast<SimState*>(state));
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNNSim::createBRAMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                           StateBase *state) const
{
    return std::make_unique<::BRAMArray>(type, count, static_cast<SimState*>(state));
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNNSim::createLLMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                         StateBase *state) const
{
    return std::make_unique<::LLMArray>(type, count, static_cast<SimState*>(state));
}
//------------------------------------------------------------------------
std::unique_ptr<ArrayBase> BackendFeNNSim::createDRAMArray(const GeNN::Type::ResolvedType &type, size_t count,
                                                           StateBase *state) const
{
    return std::make_unique<::DRAMArray>(type, count, static_cast<SimState*>(state));
}
//------------------------------------------------------------------------
std::unique_ptr<IFieldArray> BackendFeNNSim::createFieldArray(const Model &model, StateBase *state) const
{
    return std::make_unique<::BRAMFieldArray<BRAMArray>>(GeNN::Type::Uint32, model.getNumFields(), static_cast<SimState*>(state));
}
//------------------------------------------------------------------------
std::unique_ptr<StateBase> BackendFeNNSim::createState() const
{
    return std::make_unique<SimState>();
}