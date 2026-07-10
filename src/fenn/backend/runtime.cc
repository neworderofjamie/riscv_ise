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
#include "frontend/merged_model.h"
#include "frontend/process_group.h"
#include "frontend/variable.h"

// Assembler includes
#include "fenn/assembler/assembler_utils.h"

// FeNN backend includes
#include "fenn/backend/events.h"
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
std::unique_ptr<Frontend::ArrayBase> DeviceFeNN::createPerformanceCounter()
{
    LOGI_FENN_BACKEND << "Creating performance counter array in BRAM";

    // Performance counter contains a 64-bit number for 
    // instructions retired and one for number of cycles 
    return createBRAMArray(Type::Uint64, Frontend::Shape{2});
}
//----------------------------------------------------------------------------
void DeviceFeNN::createFieldArray(uint32_t numFieldBytes)
{
    LOGI_FENN_BACKEND << "Creating field array in BRAM";

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
:   Frontend::Runtime(std::make_unique<Model>(kernels), numDevices, 32), m_UseDRAMForWeights(useDRAMForWeights), 
    m_KeepParamsInRegisters(keepParamsInRegisters), m_NeuronUpdateRoundingMode(neuronUpdateRoundingMode), 
    m_DMABufferSize(dmaBufferSize)
{
    //! Same ready flag is used by all kernels and located at BRAM address zero
    constexpr uint32_t readyFlagPtr = 0;

    //! Fields always start at address 4
    uint32_t fieldBase = 4;

    // Loop through kernels
    const auto &model = getMergedModel().getModel<Model>();
    for (const auto &k : model.getKernels()) {
        // Generate kernel
        auto code = Assembler::Utils::generateStandardKernel(
            generateSimulationKernels, readyFlagPtr,
            [this, generateSimulationKernels, &fieldBase, &k, &model]
            (Assembler::CodeGenerator &c, Assembler::VectorRegisterAllocator &vectorRegisterAllocator, 
             Assembler::ScalarRegisterAllocator &scalarRegisterAllocator)
            {
                // Ensure kernel has proper base class
                auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
                if (!ki) {
                    throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
                }

                // Define jump table for routing events
                // **NOTE** this is at the top of the kernel so it can be easily addressed
                auto jumpTable = c.L();
                std::unordered_map<std::shared_ptr<const Frontend::EventSource>, Assembler::Label> eventSourceLabels;
                if(!ki->getEventSinkIDs().empty()) {
                    // Jump over jump table
                    auto endOfJumpTable = Assembler::createLabel();
                    c.j_(endOfJumpTable);

                    // **HACK**
                    if(!generateSimulationKernels) {
                        c.nop();
                    }

                    // Loop through event sink ids
                    std::vector<Assembler::Label> labels;
                    labels.resize(ki->getEventSinkIDs().size());
                    for (const auto &e : ki->getEventSinkIDs()) {
                        // **YUCK** if event sink is also an event source i.e. it's a channel
                        auto eventSource = std::dynamic_pointer_cast<const Frontend::EventSource>(e.first);
                        if(eventSource) {
                            // Create new label to jump to
                            auto label = Assembler::createLabel();

                            // Insert label at correct index in vector
                            auto &l = labels.at(e.second / 4);
                            assert(!l);
                            l = label;

                            // Add mapping between event source and label
                            eventSourceLabels.try_emplace(eventSource, label);
                        }
                    }

                    // Generate event sink jump tables
                    bool gapEncountered = false;
                    for (const auto &l : labels) {
                        if(l) {
                            assert(!gapEncountered);
                            c.j_(l);
                        }
                        else {
                            gapEncountered = true;
                        }
                    }

                    // Label at end of jump table
                    c.L(endOfJumpTable);
                }

                // Generate code for kernel
                ki->generateCode(c, scalarRegisterAllocator, vectorRegisterAllocator,
                                 [this, jumpTable, &eventSourceLabels, &fieldBase, &ki]
                                 (auto processGroup, auto timeRegister, auto numTimesteps, auto &c,
                                  auto &scalarRegisterAllocator, auto &vectorRegisterAllocator)
                                 {
                                     // Create empty vector of merged fields associated with these processes
                                     auto mergedFields = m_MergedField.emplace(std::piecewise_construct,
                                                                               std::make_tuple(processGroup),
                                                                               std::make_tuple());
                                     if (!mergedFields.second) {
                                         throw std::runtime_error("Process groups should not be used multiple times in kernels");
                                     }

                                     // Reserve merged fields for each process group
                                     const auto &mergedProcessGroup = getMergedModel().getMergedProcessGroups().at(processGroup);
                                     const auto &mergedProcesses = mergedProcessGroup.getMergedProcesses();
                                     mergedFields.first->second.reserve(mergedProcesses.size());

                                     // If this is the event source process group
                                     if (processGroup == ki->getEventSourceProcessGroup()) {
                                         ALLOCATE_SCALAR(SPreIndex);
                                         ALLOCATE_SCALAR(SGroupIndex);
                                         ALLOCATE_SCALAR(SMergedGroupReturn);
                                         ALLOCATE_SCALAR(SSpikeReturn);

                                         // Create map containing a label for each merged process (key is archectype progress group)
                                         std::unordered_map<std::shared_ptr<const Frontend::Process>,
                                                            Assembler::Label> mergedProcessLabels;
                                         std::transform(mergedProcesses.cbegin(), mergedProcesses.cend(),
                                                        std::inserter(mergedProcessLabels, mergedProcessLabels.end()),
                                                        [](const auto &m)
                                                        {
                                                            return std::make_pair(m.getArchetype(), Assembler::createLabel());
                                                        });

                                         // Loop over merged event sources and generate event processing loops
                                         // **TODO** pass through fields so event source buffer can be implemented
                                         auto endProcessGroupLabel = Assembler::createLabel();
                                         for (const auto &m : ki->getMergedEventSources()) {
                                             m.getArchetype<EventSourceImplementation>()->generateEventLoop(
                                                m, *this, *ki, SPreIndex, SSpikeReturn, jumpTable, 
                                                c, scalarRegisterAllocator);
                                         }


                                         // Wait for all routers to be reset
                                         Assembler::Utils::generateRouterBarrier(c, scalarRegisterAllocator, getNumDevices());

                                         // Jump over event handlers
                                         c.j_(endProcessGroupLabel);

                                         // Loop through event sources and their proceses
                                         for (const auto &e : ki->getEventSourceProcesses()) {
                                             // Get corresponding label and define
                                             const auto &label = eventSourceLabels.at(e.first);
                                             c.L(label);

                                             // Loop through all processes which should handle this
                                             for (const auto &p : e.second) {
                                                 // Determine this processes destination in merged groups
                                                 const auto &destination = mergedProcessGroup.getDestination(p);
                                                 
                                                 // Set group index
                                                 c.li(*SGroupIndex, destination.second);

                                                 // Jump to merged process handler, storing return address
                                                 c.jal(*SMergedGroupReturn, mergedProcessLabels.at(p));
                                             }

                                             // Return to spike loop to process next spike
                                             c.jr(*SSpikeReturn);
                                         }

                                         // Generate blocks of code to update each merged event propagation process
                                         // These code blocks are event driven so expect SPreIndex, SGroupIndex 
                                         // and SMergedGroupReturn to be populated before jumping to them
                                         for(const auto &m : mergedProcesses) {
                                             // Ensure process has proper base class
                                             auto pi = std::dynamic_pointer_cast<const ProcessImplementation>(m.getArchetype());
                                             if (!pi) {
                                                 throw std::runtime_error("FeNN backend runtime used with incompatible process");
                                             }
                                            
                                             // Define label
                                             c.L(mergedProcessLabels.at(m.getArchetype()));

                                             // Add new merged field
                                             // **NOTE** these are relative to start of field array
                                             mergedFields.first->second.emplace_back(std::piecewise_construct,
                                                                                     std::make_tuple(fieldBase - 4),
                                                                                     std::make_tuple());

                                             // Generate code
                                             pi->generateCode(m, *this, *ki, mergedFields.first->second.back().second, 
                                                              timeRegister, SPreIndex, SGroupIndex, 
                                                              numTimesteps, fieldBase, c, 
                                                              scalarRegisterAllocator, vectorRegisterAllocator);
                                             // Return
                                             c.jr(*SMergedGroupReturn);
                                         }

                                         // End of kernel
                                         c.L(endProcessGroupLabel);
                                     }
                                     // Otherwise
                                     else {
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
                                            pi->generateCode(m, *this, *ki, mergedFields.first->second.back().second, 
                                                            timeRegister, nullptr, nullptr, 
                                                            numTimesteps, fieldBase, c, 
                                                            scalarRegisterAllocator, vectorRegisterAllocator);
                                        }
                                     }
                                 });
            });

        // Add to kernel code dictionary
        m_KernelCode.try_emplace(k, code);
    }

    // Calculate number of bytes required for fields
    m_NumFieldBytes = fieldBase - 4;
    LOGI_FENN_BACKEND << m_NumFieldBytes << " bytes of BRAM required for fields";
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
        const auto &mergedProcesses = m.second.getMergedProcesses();
        assert(mergedProcesses.size() == f.size());

        LOGD_FENN_BACKEND << "Populating fields for process group '" << m.first->getName() << "'";

        // Loop through the merged processes and shared fields for this merged group
        for(size_t g = 0; g < mergedProcesses.size(); g++) {
            const auto &mergedProcess = mergedProcesses[g];
            const auto &mergedFields = f[g];

            LOGD_FENN_BACKEND << "\tMerged group " << g;

            // Loop through processes
            for(size_t p = 0; p < mergedProcess.getMerged().size(); p++) {
                // Get base address of this process's fields
                const uint32_t fieldBaseAddress = mergedFields.first + (p * mergedFields.second.getSize());

                auto process = mergedProcess.getMerged()[p];
                LOGD_FENN_BACKEND << "\t\tProcess '" << process->getName() << "'";

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
                                    LOGD_FENN_BACKEND << "\t\t\tWriting value " << v << " into field at " << fieldAddress;
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

                            LOGD_FENN_BACKEND << "\t\t\tWriting pointer into field at " << fieldAddress;

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
