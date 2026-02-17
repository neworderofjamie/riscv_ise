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
#include "model/variable.h"

// FeNN backend includes
#include "fenn/backend/model.h"

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
// FeNN::Backend::Runtime
//----------------------------------------------------------------------------
std::unique_ptr<::Backend::ArrayBase> Runtime::createArray(std::shared_ptr<const ::Model::EventContainer> eventContainer) const
{
    LOGI << "Creating event container '" << eventContainer->getName() << "' array in BRAM";

    // Event containers are always implemented as BRAM bitfields
    const size_t numSpikeWords = ::Common::Utils::ceilDivide(eventContainer->getShape().getFlattenedSize(), 32) * eventContainer->getNumBufferTimesteps();
    return createBRAMArray(GeNN::Type::Uint32, numSpikeWords);
}
//----------------------------------------------------------------------------
std::unique_ptr<::Backend::ArrayBase> Runtime::createArray(std::shared_ptr<const ::Model::PerformanceCounter> performanceCounter) const
{
    LOGI << "Creating performance counter '" << performanceCounter->getName() << "' array in BRAM";

    // Performance counter contains a 64-bit number for 
    // instructions retired and one for number of cycles 
    return createBRAMArray(GeNN::Type::Uint64, 2);
}
//----------------------------------------------------------------------------
std::unique_ptr<::Backend::ArrayBase> Runtime::createArray(std::shared_ptr<const ::Model::Variable> variable) const
{
    
    // Pad last dimension to multiplies of 32
    // **THINK** how much of this belongs in Shape?
    // **TODO** more information is required here to seperate variables with
    // shape (B,) which shouldn't be padded from (N,) variables which should
    auto varDims = variable->getShape().getDims();
    varDims.back() = ::Common::Utils::padSize(varDims.back(), 32);

    // Multiple padded dimensions together
    const size_t countOneTimestep = std::accumulate(varDims.cbegin(), varDims.cend(), 
                                                    1, std::multiplies<size_t>());
    const size_t count = countOneTimestep * variable->getNumBufferTimesteps();

    // Upcast model to FeNN-model and determine what memory-space this variable lives in
    const auto &model = dynamic_cast<const Model&>(getModelMerged().getModel());
    const auto &memSpaceCompatibility = model.getStateMemSpaceCompatibility();
    

    // Create URAM array if variable can be implemented there
    if(memSpaceCompatibility.uram) {
        LOGI << "Creating variable '" << variable->getName() << "' array in URAM";
        return createURAMArray(variable->getType(), count);
    }
    // Otherwise, create LLM array if variable can be implemented there
    else if(memSpaceCompatibility.llm) {
        LOGI << "Creating variable '" << variable->getName() << "' array in LLM";
        return createLLMArray(variable->getType(), count);
    }
    else if(memSpaceCompatibility.uramLLM) {
        LOGI << "Creating variable '" << variable->getName() << "' array in URAM and LLM";
        return createURAMLLMArray(variable->getType(), countOneTimestep, count);
    }
    // Otherwise, create BRAM array if variable can be implemented there
    else if(memSpaceCompatibility.bram) {
        LOGI << "Creating variable '" << variable->getName() << "' array in BRAM";
        return createBRAMArray(variable->getType(), count);
    }
    // Otherwise, create DRAM array if variable can be implemented there
    else if(memSpaceCompatibility.dram) {
        LOGI << "Creating variable '" << variable->getName() << "' array in DRAM";
        return createDRAMArray(variable->getType(), count);
    }
    else {
        throw std::runtime_error("Variable '" + variable->getName() + "' is not compatible "
                                 "with any memory spaces available on FeNN");
    }
}
}