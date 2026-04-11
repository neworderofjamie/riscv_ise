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
#include "frontend/events.h"
#include "frontend/process_group.h"
#include "frontend/variable.h"

// Assembler includes
#include "fenn/assembler/assembler_utils.h"

// FeNN backend includes
#include "fenn/backend/model.h"
#include "fenn/backend/process.h"
#include "fenn/backend/kernel.h"

using namespace CompilerFrontend;

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
    return createBRAMArray(Type::Uint32, wordAlignedShape);
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> DeviceFeNN::createArray(std::shared_ptr<const Frontend::Variable> variable,
                                                             const Frontend::Shape &shape)
{
    // Pad last dimension to multiplies of 32
    // **THINK** this is not correct 
    // - for neurons we just want to pad total
    // - for weighs we want to pad rows
    const auto paddedShape = shape.padLast(32);

    // Create array in correct memory space depending on compatibility
    switch(getRuntime().getModel<Model>()->getStateMemSpace(variable, getRuntime().shouldUseDRAMForWeights()))
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
    LOGI << "Creating performance counter array in BRAM";

    // Performance counter contains a 64-bit number for 
    // instructions retired and one for number of cycles 
    return createBRAMArray(Type::Uint64, Frontend::Shape{2});
}
//----------------------------------------------------------------------------
void DeviceFeNN::createFieldArray(uint32_t numFieldBytes)
{
    LOGI << "Creating field array in BRAM";

    // Create field array in BRAM and assert it is at correct fixed location
    m_FieldArray = createBRAMArray(Type::Uint8, Frontend::Shape(numFieldBytes));
    assert(m_FieldArray->getBRAMPointer() == 4);
}

//----------------------------------------------------------------------------
// FeNN::Backend::Runtime
//----------------------------------------------------------------------------
Runtime::Runtime(const std::vector<std::shared_ptr<const Frontend::Kernel>> &kernels, 
                 size_t numDevices, bool generateSimulationKernels, bool useDRAMForWeights, 
                 bool keepParamsInRegisters, Compiler::RoundingMode neuronUpdateRoundingMode, 
                 size_t dmaBufferSize)
:   Frontend::Runtime(std::make_unique<Model>(kernels), numDevices), m_UseDRAMForWeights(useDRAMForWeights), 
    m_KeepParamsInRegisters(keepParamsInRegisters), m_NeuronUpdateRoundingMode(neuronUpdateRoundingMode), 
    m_DMABufferSize(dmaBufferSize)
{
    //! Same ready flag is used by all kernels and located at BRAM address zero
    constexpr uint32_t readyFlagPtr = 0;


    //! Fields always start at address 4
    uint32_t fieldBase = 4;
    
    // Loop through kernels
    for (const auto &k : getMergedModel().getModel().getKernels()) {
        // Generate kernel
        auto code = Assembler::Utils::generateStandardKernel(
            generateSimulationKernels, readyFlagPtr,
            [&fieldBase, &k, this]
            (Assembler::CodeGenerator &c, Assembler::VectorRegisterAllocator &vectorRegisterAllocator, 
             Assembler::ScalarRegisterAllocator &scalarRegisterAllocator)
            {
                // Ensure kernel has proper base class
                auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
                if (!ki) {
                    throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
                }

                // Generate code for kernel
                ki->generateCode(c, scalarRegisterAllocator, vectorRegisterAllocator,
                                 [this, &fieldBase]
                                 (auto processGroup, auto timeRegister, auto numTimesteps, auto &codeGenerator,
                                  auto &scalarRegisterAllocator, auto &vectorRegisterAllocator)
                                 {
                                     // Create empty vector of merged fields associated with these processes
                                     auto mergedFields = m_MergedField.emplace(std::piecewise_construct,
                                                                               std::make_tuple(processGroup), 
                                                                               std::make_tuple());
                                     if(!mergedFields.second) {
                                         throw std::runtime_error("Process groups should not be used multiple times in kernels");
                                     }

                                     // Reserve merged fields for each process group
                                     const auto &mergedProcesses = getMergedModel().getMergedProcessGroups().at(processGroup);
                                     mergedFields.first->second.reserve(mergedProcesses.size());

                                     // Loop through merged processes
                                     // **TODO** need to identify whether process group is the one that contains event propagation
                                     // If it is
                                     // 1) 
                                     for(const auto &m : mergedProcesses) {
                                         // Ensure process has proper base class
                                         auto pi = std::dynamic_pointer_cast<const ProcessImplementation>(m.getArchetype());
                                         if (!pi) {
                                             throw std::runtime_error("FeNN backend runtime used with incompatible process");
                                         }

                                         // Add new merged field
                                         // **NOTE** these are relative to start of field array
                                         mergedFields.first->second.emplace_back(std::piecewise_construct,
                                                                                 std::make_tuple(fieldBase - 4),
                                                                                 std::make_tuple());

                                         // Generate code
                                         pi->generateCode(m, *this, mergedFields.first->second.back().second, 
                                                          timeRegister, numTimesteps, fieldBase, codeGenerator, 
                                                          scalarRegisterAllocator, vectorRegisterAllocator);
                                     }
                                 });
            });

        // Add to kernel code dictionary
        m_KernelCode.try_emplace(k, code);
    }

    // Calculate number of bytes required for fields
    m_NumFieldBytes = fieldBase - 4;
    LOGI << m_NumFieldBytes << " bytes of BRAM required for fields";
}
//----------------------------------------------------------------------------
void Runtime::allocatePreamble()
{
    // Loop through devices and create field arrays
    // **NOTE** this needs to happen here so they are correctly allocated at the start of BRAM
    // **YUCK** first 4 bytes used for ready flag pointer
    for(auto &d : getDevices()) {
        static_cast<DeviceFeNN*>(d.get())->getBRAMAllocator().allocate(4);
        static_cast<DeviceFeNN*>(d.get())->createFieldArray(m_NumFieldBytes);
    }
}
//----------------------------------------------------------------------------
void Runtime::allocatePostamble()
{
    // Loop through merged process groups
    for(const auto &m : getMergedModel().getMergedProcessGroups()) {
        // Get corresponding merged fields
        const auto &f = m_MergedField.at(m.first);
        assert(m.second.size() == f.size());

        LOGD << "Populating fields for process group '" << m.first->getName() << "'";

        // Loop through the merged processes and shared fields for this merged group
        for(size_t g = 0; g < m.second.size(); g++) {
            const auto &mergedProcess = m.second[g];
            const auto &mergedFields = f[g];

            LOGD << "\tMerged group " << g;

            // Loop through processes
            for(size_t p = 0; p < mergedProcess.getProcesses().size(); p++) {
                // Get base address of this process's fields
                const uint32_t fieldBaseAddress = mergedFields.first + (p * mergedFields.second.getSize());

                auto process = mergedProcess.getProcesses()[p];
                LOGD << "\t\tProcess '" << process->getName() << "'";

                // Loop through the fields in this merged group
                for(auto &f : mergedFields.second.getFields()) {
                    // If field contains a constant
                    // **TODO** CHECK THESE AREN'T OFF BY 4 AS THESE ARE OFFSETS
                    const uint32_t fieldAddress = fieldBaseAddress + f.first;
                    if(std::holds_alternative<MergedFields::GetFieldConstantFunc<>>(f.second)) {
                        auto getFieldValueFn = std::get<MergedFields::GetFieldConstantFunc<>>(f.second);

                        // Loop through devices
                        for(size_t d = 0; d < getNumDevices(); d++) {
                            // Get value for this device
                            auto deviceValue = getFieldValueFn(d, process);
                            
                            // Copy value into field array
                            auto *fieldArray = static_cast<DeviceFeNN*>(getDevices()[d].get())->getFieldArray();
                            std::visit(
                                [fieldAddress, fieldArray](auto v)
                                { 
                                    // **TODO** CHECK FIELD SIZE AGAINST sizeof(v)
                                    LOGD << "\t\t\tWriting value " << v << " into field at " << fieldAddress;
                                    std::memcpy(fieldArray->getHostPointer() + fieldAddress, 
                                                &v, sizeof(v));
                                },
                                deviceValue);
                        }
                    }
                    // Otherwise, it contains an array
                    else {
                        // Loop through devices
                        auto getFieldPointerFn = std::get<MergedFields::GetFieldPointerFunc<>>(f.second);
                        for(auto &d : getDevices()) {
                            // Get array allocated on this device
                            auto deviceArray = getFieldPointerFn(*d, process);

                            // Serialise array's 'device object'
                            std::vector<std::byte> bytes;
                            deviceArray->serialiseDeviceObject(bytes);

                            LOGD << "\t\t\tWriting pointer into field at " << fieldAddress;

                            // Memcpy bytes into field offset
                            // **TODO** CHECK FIELD SIZE AGAINST BYTES
                            auto *fieldArray = static_cast<DeviceFeNN*>(d.get())->getFieldArray();
                            std::memcpy(fieldArray->getHostPointer() + fieldAddress, 
                                        bytes.data(), bytes.size());
                        }
                    }
                }

            }
        }
    }

    // Loop through all devices and push field arrays to device
    for(auto &d : getDevices()) {
        static_cast<DeviceFeNN*>(d.get())->getFieldArray()->pushToDevice();
    }
}
}
