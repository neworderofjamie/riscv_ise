#include "fenn/backend/runtime.h"

// Standard C++ includes
#include <numeric>

// Standard C includes
#include <cstring>

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/utils.h"

// Model includes
#include "model/event_container.h"
#include "model/performance_counter.h"
#include "model/process_group.h"
#include "model/variable.h"

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
std::unique_ptr<::Backend::ArrayBase> DeviceFeNN::createArray(std::shared_ptr<const ::Model::EventContainer> eventContainer) const
{
    LOGI << "Creating event container '" << eventContainer->getName() << "' array in BRAM";

    // Event containers are always implemented as BRAM bitfields
    const size_t numSpikeWords = ::Common::Utils::ceilDivide(eventContainer->getShape().getFlattenedSize(), 32) * eventContainer->getNumBufferTimesteps();
    return createBRAMArray(GeNN::Type::Uint32, numSpikeWords);
}
//----------------------------------------------------------------------------
std::unique_ptr<::Backend::ArrayBase> DeviceFeNN::createArray(std::shared_ptr<const ::Model::PerformanceCounter> performanceCounter) const
{
    LOGI << "Creating performance counter '" << performanceCounter->getName() << "' array in BRAM";

    // Performance counter contains a 64-bit number for 
    // instructions retired and one for number of cycles 
    return createBRAMArray(GeNN::Type::Uint64, 2);
}
//----------------------------------------------------------------------------
std::unique_ptr<::Backend::ArrayBase> DeviceFeNN::createArray(std::shared_ptr<const ::Model::Variable> variable) const
{
    
    // Pad last dimension to multiplies of 32
    // **THINK** how much of this belongs in Shape?
    // **TODO** more information is required here to seperate variables with
    // shape (B,) which shouldn't be padded from (N,) variables which should
    // **TODO** split variable across cores - may need to happen in Model
    auto varDims = variable->getShape().getDims();
    varDims.back() = ::Common::Utils::padSize(varDims.back(), 32);

    // Multiple padded dimensions together
    const size_t countOneTimestep = std::accumulate(varDims.cbegin(), varDims.cend(), 
                                                    1, std::multiplies<size_t>());
    const size_t count = countOneTimestep * variable->getNumBufferTimesteps();

    // Upcast model to FeNN-model and determine what memory-space this variable lives in
    const auto &model = dynamic_cast<const Model&>(getMergedModel().getModel());
    const auto &memSpaceCompatibility = model.getStateMemSpaceCompatibility();
    
    // Create array in correct memory space depending on compatibility
    if(memSpaceCompatibility.dram && m_UseDRAMForWeights) {
        LOGI << "Creating variable '" << variable->getName() << "' array in DRAM";
        return createDRAMArray(variable->getType(), count);
    }
    else if(memSpaceCompatibility.uram) {
        LOGI << "Creating variable '" << variable->getName() << "' array in URAM";
        return createURAMArray(variable->getType(), count);
    }
    else if(memSpaceCompatibility.llm) {
        LOGI << "Creating variable '" << variable->getName() << "' array in LLM";
        return createLLMArray(variable->getType(), count);
    }
    else if(memSpaceCompatibility.uramLLM) {
        LOGI << "Creating variable '" << variable->getName() << "' array in URAM and LLM";
        return createURAMLLMArray(variable->getType(), countOneTimestep, count, 0);
    }
    else if(memSpaceCompatibility.bram) {
        LOGI << "Creating variable '" << variable->getName() << "' array in BRAM";
        return createBRAMArray(variable->getType(), count);
    }
    else {
        throw std::runtime_error("Variable '" + variable->getName() + "' is not compatible "
                                 "with any memory spaces available on FeNN");
    }
}
//----------------------------------------------------------------------------
// FeNN::Backend::Runtime
//----------------------------------------------------------------------------
Runtime::Runtime(const ::Model::Model &model, size_t numDevices, bool useDRAMForWeights , bool keepParamsInRegisters, 
                 Compiler::RoundingMode neuronUpdateRoundingMode)
:   ::Backend::Runtime(model, numDevices), m_UseDRAMForWeights(useDRAMForWeights), 
    m_KeepParamsInRegisters(keepParamsInRegisters), m_NeuronUpdateRoundingMode(neuronUpdateRoundingMode)
{
    // **TODO** fields
    // **TODO** create device state objects to start allocating BRAM for
    
    //! Same ready flag is used by all kernels and located at BRAM address zero
    constexpr uint32_t readyFlagPtr = 0;
    
    // Cast model to FenN model
    const auto &fennModel = dynamic_cast<const Model&>(model);

    // Loop through kernels
    for (const auto &k : getMergedModel().getModel().getKernels()) {
        // Generate kernel
        const auto code = Assembler::Utils::generateStandardKernel(
            true/*shouldGenerateSimulationKernels()*/, readyFlagPtr,
            [&fennModel, &k, this](Assembler::CodeGenerator &c, Assembler::VectorRegisterAllocator &vectorRegisterAllocator, 
                       Assembler::ScalarRegisterAllocator &scalarRegisterAllocator)
            {
                // Ensure kernel has proper base class
                auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
                if (!ki) {
                    throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
                }

                // Generate code for kernel
                ki->generateCode(c, scalarRegisterAllocator, vectorRegisterAllocator,
                                 [&fennModel, this]
                                 (auto processGroup, auto &codeGenerator, auto &scalarRegisterAllocator, auto &vectorRegisterAllocator)
                                 {
                                     // Loop through merged processes
                                     for(const auto &m : getMergedModel().getMergedProcessGroups().at(processGroup)) {
                                         // Ensure process has proper base class
                                         auto pi = std::dynamic_pointer_cast<const ProcessImplementation>(m.getArchetype());
                                         if (!pi) {
                                             throw std::runtime_error("FeNN backend runtime used with incompatible process");
                                         }

                                         // Generate code
                                         pi->generateCode(m, fennModel, codeGenerator, scalarRegisterAllocator,
                                                          vectorRegisterAllocator);
                                     }
                                 });
            });
    }
}
}