#include "fenn/backend/runtime.h"

// Standard C++ includes
#include <numeric>

// Standard C includes
#include <cstring>

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/utils.h"

// Frontend includes
#include "frontend/event_container.h"
#include "frontend/process_group.h"
#include "frontend/variable.h"

// Assembler includes
#include "fenn/assembler/assembler_utils.h"

// FeNN backend includes
#include "fenn/backend/model.h"
#include "fenn/backend/process.h"
#include "fenn/backend/kernel.h"

//------------------------------------------------------------------------
// FeNN::Backend::URAMArrayBase
//------------------------------------------------------------------------
namespace FeNN::Backend
{
void URAMArrayBase::serialiseDeviceObject(std::vector<std::byte> &bytes) const
{
    // Allocate 4 bytes of space
    bytes.resize(4);

    // Memcpy URAM pointer into bytes
    const uint32_t uramPointer = getURAMPointer(); 
    std::memcpy(bytes.data(), &uramPointer, 4);
}

//------------------------------------------------------------------------
// FeNN::Backend::BRAMArrayBase
//------------------------------------------------------------------------
void BRAMArrayBase::serialiseDeviceObject(std::vector<std::byte> &bytes) const
{
    // Allocate 4 bytes of space
    bytes.resize(4);

    // Memcpy BRAM pointer into bytes
    const uint32_t bramPointer = getBRAMPointer();
    std::memcpy(bytes.data(), &bramPointer, 4);
}

//------------------------------------------------------------------------
// FeNN::Backend::LLMArrayBase
//------------------------------------------------------------------------
void LLMArrayBase::serialiseDeviceObject(std::vector<std::byte> &bytes) const
{
    // Allocate 4 bytes of space
    bytes.resize(4);

    // Memcpy LLM pointer into bytes
    // **NOTE** the code wants lane addresses not array addresses
    // **THINK** would it be better to allocate 2 byte aligned?
    const uint32_t llmPointer = getLLMPointer() / 32;
    std::memcpy(bytes.data(), &llmPointer, 4);
}

//----------------------------------------------------------------------------
// FeNN::Backend::DRAMArrayBase
//----------------------------------------------------------------------------
void DRAMArrayBase::serialiseDeviceObject(std::vector<std::byte> &bytes) const
{
    // Allocate 4 bytes of space
    bytes.resize(4);

    // Memcpy DRAM pointer into bytes
    const uint32_t dramPointer = getDRAMPointer();
    std::memcpy(bytes.data(), &dramPointer, 4);
}

//----------------------------------------------------------------------------
// FeNN::Backend::URAMLLMArrayBase
//----------------------------------------------------------------------------
void URAMLLMArrayBase::serialiseDeviceObject(std::vector<std::byte> &bytes) const
{
    // Allocate 8 bytes of space
    bytes.resize(8);

    // Memcpy URAM pointer into bytes
    const uint32_t uramPointer = getURAMPointer();
    std::memcpy(bytes.data(), &uramPointer, 4);

    // Memcpy LLM pointer into bytes
    // **NOTE** the code wants lane addresses not array addresses
    // **THINK** would it be better to allocate 2 byte aligned?
    const uint32_t llmPointer = getLLMPointer() / 32;
    std::memcpy(bytes.data() + 4, &llmPointer, 4);
}

//----------------------------------------------------------------------------
// FeNN::Backend::DeviceFeNN
//----------------------------------------------------------------------------
DeviceFeNN::DeviceFeNN(size_t deviceIndex, const Runtime &runtime)
:   Frontend::DeviceBase(deviceIndex), m_Runtime(runtime)
{
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> DeviceFeNN::createArray(std::shared_ptr<const Frontend::EventContainer> eventContainer,
                                                              const Frontend::Shape &deviceShape)
{
    LOGI << "Creating event container '" << eventContainer->getName() << "' array in BRAM";

    // Event containers are implemented as word-aligned bitfields so divide and pad last axis
    auto wordAlignedShape = deviceShape;
    wordAlignedShape.getLast() = ::Common::Utils::ceilDivide(wordAlignedShape.getLast(), 32);

    // Create BRAM array
    return createBRAMArray(GeNN::Type::Uint32, wordAlignedShape);
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> DeviceFeNN::createArray(std::shared_ptr<const Frontend::Variable> variable,
                                                             const Frontend::Shape &shape)
{
    // Pad last dimension to multiplies of 32
    const auto paddedShape = shape.padLast(32);

    // Create array in correct memory space depending on compatibility
    switch(getRuntime().getModel().getStateMemSpace(variable, getRuntime().shouldUseDRAMForWeights()))
    {
    case MemSpace::DRAM:
    {
        LOGI << "Creating variable '" << variable->getName() << "' array in DRAM";
        return createDRAMArray(variable->getType(), paddedShape);
    }
    case MemSpace::URAM:
    {
        LOGI << "Creating variable '" << variable->getName() << "' array in URAM";
        return createURAMArray(variable->getType(), paddedShape);
    }
    case MemSpace::LLM:
    {
        LOGI << "Creating variable '" << variable->getName() << "' array in LLM";
        return createLLMArray(variable->getType(), paddedShape);
    }
    case MemSpace::URAM_LLM:
    {
        LOGI << "Creating variable '" << variable->getName() << "' array in URAM and LLM";
        if (paddedShape.getNumDims() < 2) {
            throw std::runtime_error("Arrays allocated in URAM and LLM are expected to have time dimension");
        }

        // Slice off time dimension from shape
        const auto oneTimestepShape = paddedShape.slice(1);

        return createURAMLLMArray(variable->getType(), oneTimestepShape, paddedShape);
    }
    case MemSpace::BRAM:
    {
        LOGI << "Creating variable '" << variable->getName() << "' array in BRAM";
        return createBRAMArray(variable->getType(), paddedShape);
    }
    default:
        throw std::runtime_error("Variable '" + variable->getName() + "' is not compatible "
                                 "with any memory spaces available on FeNN");
    }
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> DeviceFeNN::createPerformanceCounter()
{
    //LOGI << "Creating performance counter '" << performanceCounter->getName() << "' array in BRAM";

    // Performance counter contains a 64-bit number for 
    // instructions retired and one for number of cycles 
    return createBRAMArray(GeNN::Type::Uint64, Frontend::Shape{2});
}

//----------------------------------------------------------------------------
// FeNN::Backend::Runtime
//----------------------------------------------------------------------------
Runtime::Runtime(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels, 
                 size_t numDevices, bool useDRAMForWeights , bool keepParamsInRegisters, 
                 Compiler::RoundingMode neuronUpdateRoundingMode, size_t dmaBufferSize)
:   m_Model(kernels), Frontend::Runtime(m_Model, numDevices), m_UseDRAMForWeights(useDRAMForWeights), 
    m_KeepParamsInRegisters(keepParamsInRegisters), m_NeuronUpdateRoundingMode(neuronUpdateRoundingMode),
    m_DMABufferSize(dmaBufferSize)
{
    // **TODO** fields
    // **TODO** create device state objects to start allocating BRAM for
    
    //! Same ready flag is used by all kernels and located at BRAM address zero
    constexpr uint32_t readyFlagPtr = 0;
    
    // Loop through kernels
    for (const auto &k : getMergedModel().getModel().getKernels()) {
        // Generate kernel
        auto code = Assembler::Utils::generateStandardKernel(
            true/*shouldGenerateSimulationKernels()*/, readyFlagPtr,
            [&k, this](Assembler::CodeGenerator &c, Assembler::VectorRegisterAllocator &vectorRegisterAllocator, 
                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator)
            {
                // Ensure kernel has proper base class
                auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
                if (!ki) {
                    throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
                }

                // Generate code for kernel
                ki->generateCode(c, scalarRegisterAllocator, vectorRegisterAllocator,
                                 [this]
                                 (auto processGroup, auto &codeGenerator, auto &scalarRegisterAllocator, auto &vectorRegisterAllocator)
                                 {
                                     // Loop through merged processes
                                     // **TODO** need to identify whether process group is the one that contains event propagation
                                     // If it is
                                     // 1) 
                                     for(const auto &m : getMergedModel().getMergedProcessGroups().at(processGroup)) {
                                         // Ensure process has proper base class
                                         auto pi = std::dynamic_pointer_cast<const ProcessImplementation>(m.getArchetype());
                                         if (!pi) {
                                             throw std::runtime_error("FeNN backend runtime used with incompatible process");
                                         }

                                         // Generate code
                                         pi->generateCode(m, getModel(), codeGenerator, scalarRegisterAllocator,
                                                          vectorRegisterAllocator);
                                     }
                                 });
            });

        // Add to kernel code dictionary
        m_KernelCode.try_emplace(k, code);
    }
}
}
