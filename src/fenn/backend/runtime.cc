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
    // Loop through kernels
    for(const auto &k : getModel()->getKernels()) {
        // Ensure kernel has proper base class
        auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
        if (!ki) {
            throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
        }

        // If kernel has an event source process group
        if(ki->getEventSourceProcessGroup()) {
            // Create a hash map to group together processes with the same SHA1 digest
            std::unordered_map<boost::uuids::detail::sha1::digest_type, 
                std::vector<std::shared_ptr<Frontend::EventSource const>>, 
                ::Common::Utils::SHA1Hash> protoMergedEventSources;

            // Loop through all processes in event source process group
            for (const auto &p : ki->getEventSourceProcessGroup()->getProcesses()) {
                for(const auto &e : p->getAllEventSources()) {
                    // Build hash digest
                    boost::uuids::detail::sha1 hash;
                    e->updateMergeHash(hash);
                    const auto digest = hash.get_digest();

                    // Add to map
                    protoMergedEventSources[digest].push_back(e);
                }
            }

            // Reserve final merged groups vector
            auto &mergedEventSource = m_MergedEventSources[ki->getEventSourceProcessGroup()];
            mergedEventSource.reserve(protoMergedEventSources.size());


            // Construct final merged event source array
            size_t i = 0;
            for(auto &s : protoMergedEventSources) {
                mergedEventSource.emplace_back(i++, s.second);
            }
        }
    }

    //! Same ready flag is used by all kernels and located at BRAM address zero
    constexpr uint32_t readyFlagPtr = 0;

    //! Fields always start at address 4
    uint32_t fieldBase = 4;

    // Loop through kernels
    for (const auto &k : getModel()->getKernels()) {
        // Generate kernel
        auto code = Assembler::Utils::generateStandardKernel(
            generateSimulationKernels, readyFlagPtr,
            [this, generateSimulationKernels, &fieldBase, &k]
            (Assembler::CodeGenerator &c, Assembler::VectorRegisterAllocator &vectorRegisterAllocator, 
             Assembler::ScalarRegisterAllocator &scalarRegisterAllocator)
            {
                // Ensure kernel has proper base class
                auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
                if (!ki) {
                    throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
                }

                // If performance counters are enabled, disinhibit them
                // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
                // **TODO** also any real-time kernels that use performance counters
                {
                    const auto processGroups = k->getAllProcessGroups();
                    if (std::any_of(processGroups.cbegin(), processGroups.cend(),
                                    [](const auto &p) { return p->shouldRecordPerformance(); }))
                    {
                        c.csrw(Common::CSR::MCOUNTINHIBIT, Common::Reg::X0);
                    }
                }

                // Assign a label to each event source
                std::unordered_map<std::shared_ptr<const Frontend::EventSource>, Assembler::Label> eventSourceLabels;
                std::transform(ki->getEventSourceProcesses().cbegin(), ki->getEventSourceProcesses().cend(),
                               std::inserter(eventSourceLabels, eventSourceLabels.begin()),
                               [](const auto &e){ return std::make_pair(e.first, Assembler::createLabel()); });

                // Define jump table for routing events
                // **NOTE** this is at the top of the kernel so it can be easily addressed
                auto jumpTable = Assembler::createLabel();
                if(!ki->getEventSinkIDs().empty()) {
                    // Jump over jump table
                    auto endOfJumpTable = Assembler::createLabel();
                    c.j_(endOfJumpTable);

                    c.L(jumpTable);
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
                            // Insert label at correct index in vector
                            auto &l = labels.at(e.second / 4);
                            assert(!l);
                            l = eventSourceLabels.at(eventSource);
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
                                         for (const auto &e : ki->getEventSourceProcesses()) {
                                             // Get corresponding label and define
                                             const auto &label = eventSourceLabels.at(e.first);
                                             c.L(label);

                                             // Loop through all processes which should handle this
                                             for (const auto &p : e.second) {
                                                 // Determine this processes destination in merged groups
                                                 const auto &destination = mergedProcessGroup.getDestination(p);
                                                 
                                                 // Set group index
                                                 // **OPTIMISE** if there is only 1 merged process, no need for this!
                                                 c.li(*SGroupIndex, destination.second);

                                                 // Jump to merged process handler, storing return address
                                                 c.jal(*SMergedGroupReturn, mergedProcessLabels.at(destination.first));
                                             }

                                             // Return to spike loop to process next spike
                                             c.jr(*SSpikeReturn);
                                         }

                                         // Resolve addresses of event source labels
                                         // **THINK** this is required because we are generating event loop using a different code generator
                                         // Alternatively, could chain code generators together for label resolution but not clear  if that's any les gross
                                         std::unordered_map<std::shared_ptr<const Frontend::EventSource>, uint32_t> eventSourceAddresses;
                                         std::transform(eventSourceLabels.cbegin(), eventSourceLabels.cend(),
                                                        std::inserter(eventSourceAddresses, eventSourceAddresses.begin()),
                                                        [&c](const auto &e){ return std::make_pair(e.first, c.getAddress(e.second).value()); });

                                         // Loop over merged event sources
                                         Assembler::CodeGenerator eventLoopCodeGenerator;
                                         const auto &mergedEventSourcesGroup = getMergedEventSources().at(processGroup);
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
                                                 timeRegister, SPreIndex, SSpikeReturn, c.getAddress(jumpTable).value(), eventSourceAddresses,
                                                 fieldBase, eventLoopCodeGenerator, scalarRegisterAllocator);
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
                auto deviceArray = getFieldPointerFn(*d, owner);

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

    // Loop through merged event sources
    for(const auto &m : getMergedEventSources()) {
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

    // Loop through all devices and push field arrays to device
    for(auto &d : getDevices()) {
        static_cast<DeviceFeNN*>(d.get())->getFieldArray()->pushToDevice();
    }
}
}
