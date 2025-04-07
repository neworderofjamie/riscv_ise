#include "backend/backend_fenn_sim.h"

// Standard C++ includes
#include <stdexcept>

// Plog includes
#include <plog/Log.h>

// Common includes
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
    SimState()
    {
        m_RISCV.addCoprocessor<VectorProcessor>(vectorQuadrant);
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

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    RISCV m_RISCV;
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
// BRAMArray
//------------------------------------------------------------------------
class BRAMFieldArray : public IFieldArray, protected BRAMArray
{
public:
    using BRAMArray::BRAMArray;

    //------------------------------------------------------------------------
    // IFieldArray virtuals
    //------------------------------------------------------------------------
    //! Sets field at offset to point to array
    virtual void setFieldArray(uint32_t fieldOffset, const ArrayBase *array) override final
    {
        // Serialise array's 'device object'
        std::vector<std::byte> bytes;
        array->serialiseDeviceObject(bytes);

        // Check serialised data is exactly 4 bytes
        assert(bytes.size() == 4);

        // Memcpy bytes into field offset
        std::memcpy(getHostPointer() + fieldOffset, 
                    bytes.data(), 4);
            
    }

    //! Copy field data to device
    virtual void pushFieldsToDevice() override final
    {
        pushToDevice();
    }
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
std::unique_ptr<IFieldArray> BackendFeNNSim::createFieldArray(const Model &model, StateBase *state) const
{
    return std::make_unique<::BRAMFieldArray>(GeNN::Type::Uint32, model.getNumFields(), static_cast<SimState*>(state));
}
//------------------------------------------------------------------------
std::unique_ptr<StateBase> BackendFeNNSim::createState() const
{
    return std::make_unique<SimState>();
}