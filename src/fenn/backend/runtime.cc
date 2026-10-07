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
#include "frontend/process_group.h"

// FeNN assembler includes
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
    const uint32_t uramPointer = getURAMPointer().value_or(0); 
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
    const uint32_t bramPointer = getBRAMPointer().value_or(0);
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
    const uint32_t llmPointer = getLLMPointer().value_or(0) / 32;
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
    const uint32_t dramPointer = getDRAMPointer().value_or(0);
    std::memcpy(bytes.data(), &dramPointer, 4);
}

//----------------------------------------------------------------------------
// FeNN::Backend::DeviceFeNN
//----------------------------------------------------------------------------
DeviceFeNN::DeviceFeNN(size_t deviceIndex, Runtime &runtime)
:   Frontend::DeviceBase(deviceIndex), m_Runtime(runtime)
{
}
//----------------------------------------------------------------------------
std::unique_ptr<Frontend::ArrayBase> DeviceFeNN::createPerformanceCounter()
{
    LOGI_FENN_BACKEND << "Creating performance counter array in BRAM";

    // Performance counter contains a 64-bit number for 
    // instructions retired and one for number of cycles 
    return createBRAMArray(Type::Uint64, {2}, {Type::Uint64.getSize(4)});
}
//----------------------------------------------------------------------------
void DeviceFeNN::createFieldArray(uint32_t numFieldBytes)
{
    LOGI_FENN_BACKEND << "Creating field array in BRAM";

    // Create field array in BRAM and assert it is at correct fixed location
    m_FieldArray = createBRAMArray(Type::Uint8, {numFieldBytes}, {Type::Uint8.getSize()});
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
    // Loop through kernels
    uint32_t fieldBase = 4;
    std::unordered_map<std::shared_ptr<const Frontend::Kernel>, 
                       std::tuple<uint32_t, uint32_t>> kernelEventTableLocations;
    for (const auto &k : getModel()->getKernels()) {
        // Ensure kernel has proper base class
        auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
        if (!ki) {
            throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
        }

        // Add size of datastructures to fieldbase
        const uint32_t eventSinkSourceTableAddress = fieldBase;
        fieldBase += (ki->getEventSinkSourceTable().size() * 2);
        LOGD_FENN_BACKEND << "Kernel '" << k->getName() << "' has an event sink source table at " << eventSinkSourceTableAddress << " and an event source->process table at " << fieldBase;
        kernelEventTableLocations.try_emplace(k, eventSinkSourceTableAddress, fieldBase);
        fieldBase += (ki->getEventSourceProcessTableSize() * 4);
    }
    // Ensure field-base is 32-bit word aligned
    fieldBase = ::Common::Utils::padSize(fieldBase, 4);

    LOGD_FENN_BACKEND << "Merged fields start at " << fieldBase;

    //! Same ready flag is used by all kernels and located at BRAM address zero
    constexpr uint32_t readyFlagPtr = 0;

    // Loop through kernels
    for (const auto &k : getModel()->getKernels()) {
        // Generate kernel
        auto code = Assembler::Utils::generateStandardKernel(
            generateSimulationKernels, readyFlagPtr,
            [this, generateSimulationKernels, &fieldBase, &k, &kernelEventTableLocations]
            (Assembler::CodeGenerator &c, Assembler::VectorRegisterAllocator &vectorRegisterAllocator, 
             Assembler::ScalarRegisterAllocator &scalarRegisterAllocator)
            {
                // Ensure kernel has proper base class
                auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
                if (!ki) {
                    throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
                }

                // If performance counters are required for this kernel, disinhibit them
                if(ki->requiresPerformanceCounters()) {
                    c.csrw(Common::CSR::MCOUNTINHIBIT, Common::Reg::X0);
                }

                // Generate code for kernel
                ki->generateCode(c, scalarRegisterAllocator, vectorRegisterAllocator,
                                 [this, &fieldBase, &k, &kernelEventTableLocations, &ki]
                                 (auto processGroup, auto timeRegister, auto numTimesteps, auto &c,
                                  auto &scalarRegisterAllocator, auto &vectorRegisterAllocator)
                                 {
                                     // Create empty vector of merged fields associated with these processes
                                     auto mergedProcessGroupFields = m_MergedProcessFields.emplace(std::piecewise_construct,
                                                                                                   std::make_tuple(processGroup),
                                                                                                   std::make_tuple());
                                     if (!mergedProcessGroupFields.second) {
                                         throw std::runtime_error("Process groups should not be used multiple times in kernels");
                                     }

                                     // Reserve merged fields for each process group
                                     const auto &mergedProcessGroup = getMergedProcessGroups().at(processGroup);
                                     const auto &mergedProcesses = mergedProcessGroup.getMergedProcesses();
                                     mergedProcessGroupFields.first->second.reserve(mergedProcesses.size());

                                     // If this is the event source process group
                                     if (processGroup == ki->getEventSourceProcessGroup()) {
                                         const auto &eventTableLocations = kernelEventTableLocations.at(k);

                                         // Create map containing a label for each merged process (key is archectype progress group)
                                         std::unordered_map<std::shared_ptr<const Frontend::Process>,
                                                            Assembler::Label> mergedProcessLabels;
                                         std::transform(mergedProcesses.cbegin(), mergedProcesses.cend(),
                                                        std::inserter(mergedProcessLabels, mergedProcessLabels.end()),
                                                        [](const auto &m)
                                                        {
                                                            return std::make_pair(m.getArchetype(), Assembler::createLabel());
                                                        });
                                         
                                         // Create empty vector of merged fields associated with these event sources
                                         auto mergedEventSourceFields = m_MergedEventSourceFields.emplace(
                                             std::piecewise_construct, std::make_tuple(processGroup),
                                             std::make_tuple());
                                
                                         if (!mergedEventSourceFields.second) {
                                             throw std::runtime_error("Process groups should not be used multiple times in kernels");
                                         }

                                         // Jump over event handlers
                                         auto endProcessGroupLabel = Assembler::createLabel();
                                         c.j_(endProcessGroupLabel);

                                         // Loop through event sources and their proceses
                                         // **TODO** potentially this could happen much later
                                         /*auto &eventSourceProcesses = m_KernelEventSourceProcesses.at(k);
                                         const auto &eventSourceProcessOffsets = kernelEventSourceProcessOffsets.at(k);
                                         for (const auto &e : ki->getEventSourceProcesses()) {
                                             // Loop through all processes which should handle this
                                             uint32_t processOffset = eventSourceProcessOffsets.at(e.first);
                                             for (const auto &p : e.second) {
                                                 // Determine this processes destination in merged groups
                                                 const auto &destination = mergedProcessGroup.getDestination(p);

                                                 // Populate event source process table
                                                 auto &eventSourceProcess = eventSourceProcesses.at(processOffset);
                                                 eventSourceProcess.eventPropCodeAddr = mergedProcessLabels.at(destination.first);
                                                 eventSourceProcess.eventPropMergedGroupIndex = destination.second;
                                             }
                                         }*/

                                         // Registers used to communicate presynaptic spike id, group index 
                                         // and return address from event loop to event propagation proces code
                                         ALLOCATE_SCALAR(SPreIndex);
                                         ALLOCATE_SCALAR(SGroupIndex);
                                         ALLOCATE_SCALAR(SMergedGroupReturn);
                        

                                         // Loop over merged event sources
                                         Assembler::CodeGenerator eventLoopCodeGenerator;
                                         const auto &mergedEventSourcesGroup = ki->getMergedEventSources().at(processGroup);
                                         mergedEventSourceFields.first->second.reserve(mergedEventSourcesGroup.size());
                                         uint32_t eventLoopScalarRegisterMask = 0;
                                         for (const auto &m : mergedEventSourcesGroup) {
                                             // Add new merged field
                                             // **NOTE** these are relative to start of field array
                                             // **TODO** pass through fields so event source buffer can be implemented
                                             mergedEventSourceFields.first->second.emplace_back(std::piecewise_construct,
                                                                                                std::make_tuple(fieldBase - 4),
                                                                                                std::make_tuple());

                                             // Generate event processing loops
                                             eventLoopScalarRegisterMask |= m.template getArchetype<EventSourceImplementation>()->generateEventLoop(
                                                 m, *this, *ki, mergedEventSourceFields.first->second.back().second,
                                                 timeRegister, SPreIndex, std::get<0>(eventTableLocations),
                                                 fieldBase, eventLoopCodeGenerator, scalarRegisterAllocator,
                                                 [SGroupIndex, SMergedGroupReturn, &eventTableLocations, &scalarRegisterAllocator]
                                                 (auto &c, auto eventSourceProcessStartOffsetReg, auto eventSourceProcessEndOffsetReg)
                                                 {
                                                     // Loop over event propagation processes
                                                     {
                                                         auto processLoop = c.L();

                                                         // Load process address and group index from event source->process table
                                                         ALLOCATE_SCALAR(SProcessAddress);
                                                         c.lhu(*SProcessAddress, *eventSourceProcessStartOffsetReg, std::get<1>(eventTableLocations));
                                                         c.lhu(*SGroupIndex, *eventSourceProcessStartOffsetReg, std::get<1>(eventTableLocations) + 2);

                                                         // Jump to process address
                                                         c.jalr(*SMergedGroupReturn, *SProcessAddress);

                                                         // Advance pointer
                                                         c.addi(*eventSourceProcessStartOffsetReg, *eventSourceProcessStartOffsetReg, 4);

                                                         // Loop if there are more events
                                                         c.bne(*eventSourceProcessStartOffsetReg, *eventSourceProcessEndOffsetReg, processLoop);
                                                     }
                                                 });
                                         }

                                         LOGD_FENN_BACKEND << "Event loops require " << ::Common::Utils::popCount(eventLoopScalarRegisterMask) << " scalar registers";

                                         // Mask registers required for event loops
                                         scalarRegisterAllocator.maskRegisters(eventLoopScalarRegisterMask);

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
                                             mergedProcessGroupFields.first->second.emplace_back(std::piecewise_construct,
                                                                                                 std::make_tuple(fieldBase - 4),
                                                                                                 std::make_tuple());

                                             // Generate code
                                             pi->generateCode(m, *this, *ki, mergedProcessGroupFields.first->second.back().second, 
                                                              timeRegister, SPreIndex, SGroupIndex, 
                                                              numTimesteps, fieldBase, c, 
                                                              scalarRegisterAllocator, vectorRegisterAllocator);
                                             // Return
                                             c.jr(*SMergedGroupReturn);
                                         }

                                         // Unmask event loop registers
                                         scalarRegisterAllocator.unmaskRegisters(eventLoopScalarRegisterMask);

                                         // End of kernel
                                         c.L(endProcessGroupLabel);

                                         // Add generted event loop code
                                         c += eventLoopCodeGenerator;

                                         
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
                                            mergedProcessGroupFields.first->second.emplace_back(std::piecewise_construct,
                                                                                    std::make_tuple(fieldBase - 4),
                                                                                    std::make_tuple());

                                            // Generate code
                                            pi->generateCode(m, *this, *ki, mergedProcessGroupFields.first->second.back().second, 
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
void Runtime::populateFields(size_t p, const std::pair<uint32_t, MergedFields> &mergedFields,
                             std::shared_ptr<const Frontend::ModelComponent> owner)
{
    // Get base address of this process's fields
    const uint32_t fieldBaseAddress = mergedFields.first + (p * mergedFields.second.getSize());

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
                auto deviceValue = getFieldValueFn(d, owner);
                
                // Copy value into field array
                auto *fieldArray = static_cast<DeviceFeNN*>(getDevices()[d].get())->getFieldArray();
                std::visit(
                    [fieldAddress, fieldArray](auto v)
                    {
                        LOGD_FENN_BACKEND << "\t\t\tWriting value " << v << " into field at " << fieldAddress;
                        static_assert(sizeof(v) <= 4, "In FeNN backend, fields are always 4 bytes");
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
                auto deviceArray = getFieldPointerFn(*d, owner);

                // Serialise array's 'device object'
                std::vector<std::byte> bytes;
                deviceArray->serialiseDeviceObject(bytes);
                assert(bytes.size() <= 4);
                LOGD_FENN_BACKEND << "\t\t\tWriting pointer into field at " << fieldAddress;

                // Memcpy bytes into field offset
                auto *fieldArray = static_cast<DeviceFeNN*>(d.get())->getFieldArray();
                std::memcpy(fieldArray->getHostPointer() + fieldAddress, 
                            bytes.data(), bytes.size());
            }
        }
    }
}
//----------------------------------------------------------------------------
void Runtime::allocatePostamble()
{
    // Loop through merged process groups
    for(const auto &m : getMergedProcessGroups()) {
        // Get corresponding merged fields
        const auto &f = m_MergedProcessFields.at(m.first);
        const auto &mergedProcesses = m.second.getMergedProcesses();
        assert(mergedProcesses.size() == f.size());

        LOGD_FENN_BACKEND << "Populating fields associated with processes in process group '" << m.first->getName() << "'";

        // Loop through the merged processes and shared fields for this merged group
        for(size_t g = 0; g < mergedProcesses.size(); g++) {
            const auto &mergedProcess = mergedProcesses[g];
            const auto &mergedFields = f[g];

            LOGD_FENN_BACKEND << "\tMerged group " << g;

            // Loop through processes
            for(size_t p = 0; p < mergedProcess.getMerged().size(); p++) {
                auto process = mergedProcess.getMerged()[p];
                LOGD_FENN_BACKEND << "\t\tProcess '" << process->getName() << "'";
                
                populateFields(p, mergedFields, process);
            }
        }
    }

    // Loop through kernels
    for (const auto &k : getModel()->getKernels()) {
        // Ensure kernel has proper base class
        auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
        if (!ki) {
            throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
        }

        // Loop through merged event sources
        for(const auto &m : ki->getMergedEventSources()) {
            // Get corresponding merged fields
            const auto &f = m_MergedEventSourceFields.at(m.first);
            const auto &mergedEventSources = m.second;
            assert(mergedEventSources.size() == f.size());

            LOGD_FENN_BACKEND << "Populating fields associated with event sources in process group '" << m.first->getName() << "'";

            // Loop through the merged processes and shared fields for this merged group
            for(size_t g = 0; g < mergedEventSources.size(); g++) {
                const auto &mergedEventSource = mergedEventSources[g];
                const auto &mergedFields = f[g];

                LOGD_FENN_BACKEND << "\tMerged event source " << g;

                // Loop through processes
                for(size_t p = 0; p < mergedEventSource.getMerged().size(); p++) {
                    auto eventSource = mergedEventSource.getMerged()[p];
                    LOGD_FENN_BACKEND << "\t\tEvent source '" << eventSource->getName() << "'";
                
                    populateFields(p, mergedFields, eventSource);
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
