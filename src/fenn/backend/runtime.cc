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
using namespace FeNN;

//-------------------------------------------------
// Anonymous namespace
//-------------------------------------------------
namespace
{
class PerformanceCounterScope
{
public:
    PerformanceCounterScope(size_t processGroupStartOffset, 
                            Assembler::CodeGenerator &codeGenerator,
                            Assembler::ScalarRegisterAllocator &scalarRegisterAllocator,
                            const std::set<std::string> &performanceCounters)
    :   m_CodeGenerator(codeGenerator), m_ScalarRegisterAllocator(scalarRegisterAllocator)
    {
        auto &c = m_CodeGenerator.get();

        // Loop through performance counters
        m_PerformanceCounters.reserve(performanceCounters.size());
        for(const auto &p : performanceCounters) {
            // Get low and high CSRs
            auto [lowCSR, highCSR] = s_PerformanceCounterCSRs.at(p);

            // Allocate registers
            auto startLowReg = m_ScalarRegisterAllocator.get().getRegister(("Start" + p + " X").c_str());
            auto startHighReg = m_ScalarRegisterAllocator.get().getRegister(("Start" + p + "H X").c_str());

            // Read performance counters into registers
            c.csrr(*startLowReg, lowCSR);
            c.csrr(*startHighReg, highCSR);

            // Add registers to vector and update offset
            m_PerformanceCounters.emplace_back(startLowReg, startHighReg, 
                                               lowCSR, highCSR,
                                               processGroupStartOffset);
            processGroupStartOffset += sizeof(uint64_t);
        }
    }

    ~PerformanceCounterScope()
    {
        auto &c = m_CodeGenerator.get();
        auto &scalarRegisterAllocator = m_ScalarRegisterAllocator.get();
            
        // Loop through performance counters
        for(const auto &p : m_PerformanceCounters) {
            auto [startLowReg, startHighReg, lowCSR, highCSR, offset] = p;

            // Read performance counters into registers
            ALLOCATE_SCALAR(SEndLow);
            ALLOCATE_SCALAR(SEndHigh);
            c.csrr(*SEndLow, lowCSR);
            c.csrr(*SEndHigh, highCSR);

            // Subtract previous performance counter values 
            // **NOTE** result goes into SEndLow and SEndHigh
            Assembler::Utils::generateSubtractUint64(c, scalarRegisterAllocator,
                                                     *SEndLow, *SEndHigh,
                                                     *startLowReg, *startHighReg);
            
            // Load previous performance counter values
            c.lw(*startLowReg, FeNN::Common::Reg::X0, offset);
            c.lw(*startHighReg, FeNN::Common::Reg::X0, offset + 4);

            // Add new performance counter values
            // **NOTE** result goes into startLowReg and startHighReg
            // **NOTE** first instruction access startLowReg so no stall
            Assembler::Utils::generateAddUint64(c, scalarRegisterAllocator,
                                                *startLowReg, *startHighReg,
                                                *SEndLow, *SEndHigh);
            // Store updated values
            c.sw(*startLowReg, FeNN::Common::Reg::X0, offset);
            c.sw(*startHighReg, FeNN::Common::Reg::X0, offset + 4);
        }
    }
private:
    //------------------------------------------------------------------------
    // Static members
    //------------------------------------------------------------------------
    inline static const std::unordered_map<std::string,
                                           std::tuple<FeNN::Common::CSR,
                                                      FeNN::Common::CSR>>
        s_PerformanceCounterCSRs = {{"num_instructions", {FeNN::Common::CSR::MCYCLE, FeNN::Common::CSR::MCYCLEH}},
                                    {"num_clock_cycles", {FeNN::Common::CSR::MINSTRET, FeNN::Common::CSR::MINSTRETH}}};

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::reference_wrapper<Assembler::CodeGenerator> m_CodeGenerator;
    std::reference_wrapper<Assembler::ScalarRegisterAllocator> m_ScalarRegisterAllocator;

    std::vector<std::tuple<Assembler::ScalarRegisterPtr, Assembler::ScalarRegisterPtr, 
                           FeNN::Common::CSR, FeNN::Common::CSR, size_t>> m_PerformanceCounters;
};
}
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
uint64_t DeviceFeNN::readPerformanceCounter(std::shared_ptr<const Frontend::ProcessGroup> group,
                                            const std::string &counter) const
{
    // Get offset of counter
    const size_t arrayOffset = (getRuntime().getModel<Model>()->getPerformanceCounterArrayOffset(group) 
                                * sizeof(uint64_t));

    // Loop through performance counters
    // **NOTE** we could store offsets in a dictionary or whatever but this is probably fine
    auto p = group->getPerformanceCounters().cbegin();
    for (size_t i = 0; i < group->getPerformanceCounters().size(); i++, p++) {
        if (*p == counter) {
            return m_PerformanceCounters->readDevice<uint64_t>(arrayOffset);
        }
    }

    throw std::runtime_error("Process group '" + group->getName() + "' missing performance counter '" + counter + "'");
}
//----------------------------------------------------------------------------
void DeviceFeNN::allocateDataStructures()
{
    // **NOTE** this needs to happen here so they are correctly allocated at the start of BRAM
    // **YUCK** first 4 bytes used for ready flag pointer
    getBRAMAllocator().allocate(4);

    // Loop through kernels
    for (const auto &k : getRuntime().getModel()->getKernels()) {
        // Ensure kernel has proper base class
        auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
        if (!ki) {
            throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
        }

        // If this kernel has an event sink-source table
        if (!ki->getEventSinkSourceTable().empty()) {
            // Create array in BRAM
            LOGD_FENN_BACKEND << "Creating event sink-source table array for kernel '" << k->getName() << "' in BRAM";
            m_KernelEventSinkSourceTableArrays.try_emplace(
                k, createBRAMArray(Type::Uint16, {ki->getEventSinkSourceTable().size()},
                                   {Type::Uint16.getSize()}));
        }

        // If this kernel has a event source->process table, allocate memory 
        if (ki->getEventSourceProcessTableSize() > 0) {
            // Create array in BRAM
            LOGD_FENN_BACKEND << "Creating event source-population table array for kernel '" << k->getName() << "' in BRAM";
            m_KernelEventSourcePopulationTableArrays.try_emplace(
                k, createBRAMArray(Type::Uint32, {ki->getEventSourceProcessTableSize() * 4},
                                   {Type::Uint32.getSize()}));
        }
    }

    // Create performance counter array
    const size_t numPerformanceCounters = getRuntime().getModel<Model>()->getPerformanceCounterArraySize();
    if(numPerformanceCounters > 0) {
        LOGI_FENN_BACKEND << "Creating performance counter array";
        m_PerformanceCounters = createBRAMArray(Type::Uint64, {numPerformanceCounters}, 
                                                {Type::Uint64.getSize()});
    }

    // Create field array in BRAM and assert it is at correct fixed location
    LOGI_FENN_BACKEND << "Creating field array in BRAM";
    m_FieldArray = createBRAMArray(Type::Uint8, {getRuntime().getNumFieldBytes()}, 
                                   {Type::Uint8.getSize()});

    // If DMA buffers are required, allocate arrays in URAM
    if (getRuntime().getDMARowBufferSize() > 0) {
        LOGI_FENN_BACKEND << "Creating DMA row buffers in URAM";
        m_DMARowBufferA = createURAMArray(Type::Int16, {getRuntime().getDMARowBufferSize() / 2},
                                          {Type::Int16.getSize()});
        m_DMARowBufferB = createURAMArray(Type::Int16, {getRuntime().getDMARowBufferSize() / 2},
                                          {Type::Int16.getSize()});

        assert(m_DMARowBufferA->getURAMPointer() == 0);
        assert(m_DMARowBufferB->getURAMPointer() == getRuntime().getDMARowBufferSize());
    }
}
//----------------------------------------------------------------------------
void DeviceFeNN::populateDataStructures()
{
    // Loop through kernels
    for (const auto &k : getRuntime().getModel()->getKernels()) {
        // Ensure kernel has proper base class
        auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
        if (!ki) {
            throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
        }

        // If this kernel has an event sink-source table
        if (!ki->getEventSinkSourceTable().empty()) {
            // Create array in BRAM
            auto *array = m_KernelEventSinkSourceTableArrays.at(k).get();

            // Convert offsets in sink source table into bytes and copy into arrray
            std::transform(ki->getEventSinkSourceTable().cbegin(), ki->getEventSinkSourceTable().cend(), 
                           array->getHostPointer<uint16_t>(), [](size_t offset){ return (offset * 16); });

            // Copy array to device
            array->pushToDevice();
        }

        // If this kernel has a event source->process table, allocate memory 
        if (ki->getEventSourceProcessTableSize() > 0) {
            // Create array in BRAM
            LOGD_FENN_BACKEND << "Populating event source-population table array for kernel '" << k->getName() << "' in BRAM";
            auto *array = m_KernelEventSourcePopulationTableArrays.at(k).get();

            // Get merged processes associated with event source process group
            assert(ki->getEventSourceProcessGroup());
            const auto &mergedProcessGroup = getRuntime().getMergedProcessGroups().at(ki->getEventSourceProcessGroup());

            // Loop through event source processes
            // **TODO** this is BASICALLY now fields, we might as well add to event loop
            const auto &mergedProcessAddresses = getRuntime().getKernelMergedProcessAddresses().at(k);
            uint32_t *arrayPointer = array->getHostPointer<uint32_t>();
            for (const auto &e : ki->getEventSourceProcesses()) {
                LOGD_FENN_BACKEND << "\t Event source: '" << e.first->getName() << "'";
                auto [processStartOffset, processEndOffset]  = ki->getEventSourceProcessTableOffsets().at(e.first);

                // Loop through processes 
                for (const auto &p : e.second) {
                    // Ensure process has proper base class
                    auto pi = std::dynamic_pointer_cast<const EventDrivenProcessImplementation>(p);
                    if (!pi) {
                        throw std::runtime_error("FeNN backend runtime used with incompatible process");
                    }

                    // Determine this processes destination in merged groups
                    const auto &destination = mergedProcessGroup.getDestination(p);

                    // Write merged process addresses and merged group index into array
                    // **NOTE** offsets are in terms of entries - each of which is two halfwords
                    arrayPointer[processStartOffset * 4] = mergedProcessAddresses.at(destination.first);
                    arrayPointer[(processStartOffset * 4) + 1] = destination.second;
                    arrayPointer[(processStartOffset * 4) + 2] = pi->getWeightStride(getRuntime(), getDeviceIndex());
                    if(pi->getWeight()) {
                        // Serialise weight array's 'device object'
                        std::vector<std::byte> bytes;
                        const auto *weightArray = getArray(pi->getWeight());
                        weightArray->serialiseDeviceObject(bytes);
                        assert(bytes.size() <= 4);

                        // Copy into array
                        std::memcpy(arrayPointer + ((processStartOffset * 4) + 3),
                                    bytes.data(), bytes.size());
                    }
                    processStartOffset++;
                    //LOGD_FENN_BACKEND << "\t Process '" << p->getName() << "': ";
                    
                }
            }

            // Copy array to device
            array->pushToDevice();
        }
    }

    // If performance counters are allocated, zero
    if(m_PerformanceCounters) {
        m_PerformanceCounters->memsetHostPointer(0);
        m_PerformanceCounters->pushToDevice();
    }
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
    m_DMABufferSize(dmaBufferSize), m_DMARowBufferSize(0)
{
    // Loop through kernels to figure out where data structures will 
    // be so immediate addresses can be written into kernels 
    uint32_t fieldStart = 4;
    std::unordered_map<std::shared_ptr<const Frontend::Kernel>, 
                       std::tuple<uint32_t, uint32_t>> kernelEventTableLocations;
    for (const auto &k : getModel()->getKernels()) {
        // Ensure kernel has proper base class
        auto ki = std::dynamic_pointer_cast<const KernelImplementation>(k);
        if (!ki) {
            throw std::runtime_error("FeNN backend runtime used with incompatible kernel");
        }

        // Add size of datastructures to fieldStart
        // **NOTE** BRAM allocations are word-aligned
        const uint32_t eventSinkSourceTableAddress = fieldStart;
        fieldStart += ::Common::Utils::padSize(ki->getEventSinkSourceTable().size() * 2, 4);
        LOGD_FENN_BACKEND << "Kernel '" << k->getName() << "' has an event sink source table at " << eventSinkSourceTableAddress << " and an event source->process table at " << fieldStart;
        kernelEventTableLocations.try_emplace(k, eventSinkSourceTableAddress, fieldStart);
        fieldStart += ::Common::Utils::padSize(ki->getEventSourceProcessTableSize() * 16, 4);

        if(shouldUseDRAMForWeights()) {
            // Loop through event source processes in the kernel
            // **YUCK** I kinda think this should go somewhere else!
            for (const auto &e : ki->getEventSourceProcesses()) {
                for(auto p : e.second) {
                    // Ensure process has proper base class
                    auto pi = std::dynamic_pointer_cast<const EventDrivenProcessImplementation>(p);
                    if (!pi) {
                        throw std::runtime_error("FeNN backend runtime used with incompatible process");
                    }
                
                    // Loop through devices and update row buffer size
                    for(size_t d = 0; d < getNumDevices(); d++) {
                        m_DMARowBufferSize = std::max(m_DMARowBufferSize, pi->getWeightStride(*this, d));
                    }
                }
            }
        }
    }

    // Add size of any performance counters to fieldStart
    uint32_t performanceCounterBase = 0;
    const size_t numPerformanceCounters = getModel<Model>()->getPerformanceCounterArraySize();
    if(numPerformanceCounters > 0) {
        LOGD_FENN_BACKEND << "Performance counters start at " << fieldStart;
        performanceCounterBase = fieldStart;
        fieldStart += (numPerformanceCounters * sizeof(uint64_t));
    }

    LOGD_FENN_BACKEND << "Merged fields start at " << fieldStart;
    uint32_t fieldBase = fieldStart;

    if (shouldUseDRAMForWeights()) {
        LOGI << "DMA row buffer size: " << m_DMARowBufferSize << " bytes";
    }

    //! Same ready flag is used by all kernels and located at BRAM address zero
    constexpr uint32_t readyFlagPtr = 0;

    // Loop through kernels
    for (const auto &k : getModel()->getKernels()) {
        // Generate kernel
        auto code = Assembler::Utils::generateStandardKernel(
            generateSimulationKernels, readyFlagPtr,
            [this, fieldStart, generateSimulationKernels, performanceCounterBase, &fieldBase, &k, &kernelEventTableLocations]
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
                                 [this, fieldStart, generateSimulationKernels, performanceCounterBase, &fieldBase, &k, &kernelEventTableLocations, &ki]
                                 (auto processGroup, auto timeRegister, auto numTimesteps, auto &c,
                                  auto &scalarRegisterAllocator, auto &vectorRegisterAllocator)
                                 {
                                    // Create performance counter scope
                                    PerformanceCounterScope p(performanceCounterBase + (getModel<Model>()->getPerformanceCounterArrayOffset(processGroup) * sizeof(uint64_t)),
                                                              c, scalarRegisterAllocator, processGroup->getPerformanceCounters());

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

                                         // **HACK** add NOP to workaround issue with jumping to jump instructions
                                         if(!generateSimulationKernels) {
                                            c.nop();
                                         }
                                         c.j_(endProcessGroupLabel);

                                         // Registers used to communicate presynaptic spike id, group index 
                                         // and return address from event loop to event propagation proces code
                                         ALLOCATE_SCALAR(SPreIndReg);
                                         ALLOCATE_SCALAR(SWeightBuffer);
                                         ALLOCATE_SCALAR(SStride);
                                         ALLOCATE_SCALAR(SGroupIndex);
                                         ALLOCATE_SCALAR(SMergedGroupReturn);
                                        
                                         // If we're using DRAM for weight
                                         Assembler::ScalarRegisterPtr SDMARowBufferA;
                                         Assembler::ScalarRegisterPtr SDMARowBufferB;
                                         if (shouldUseDRAMForWeights()) {
                                             // Allocate scalar 
                                             // Registers to hold DMA row buffer addresses
                                             SDMARowBufferA = scalarRegisterAllocator.getRegister("SDMARowBufferA = X");
                                             SDMARowBufferB = scalarRegisterAllocator.getRegister("SDMARowBufferB = X");

                                             // Load URAM addresses
                                             // **NOTE** it doesn't matter that these get swapped round
                                             c.li(*SDMARowBufferA, 0);
                                             c.li(*SDMARowBufferB, getDMARowBufferSize());
                                         }

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
                                                                                                std::make_tuple(fieldBase - fieldStart),
                                                                                                std::make_tuple());

                                             // If we should use DRAM for weights
                                             if(shouldUseDRAMForWeights()) {
                                                assert(false);
                                                 /*eventLoopScalarRegisterMask |= m.template getArchetype<EventSourceImplementation>()->generateDRAMEventLoop(
                                                     m, *this, *ki, mergedEventSourceFields.first->second.back().second,
                                                     timeRegister, std::get<0>(eventTableLocations),
                                                     fieldBase, eventLoopCodeGenerator, scalarRegisterAllocator,
                                                     // Prefetch
                                                     [SGroupIndex, SMergedGroupReturn, SDMARowBufferA, SDMARowBufferB, SStride, SWeightBuffer, &eventTableLocations, &scalarRegisterAllocator]
                                                     (auto &c, auto eventSourceProcessOffsetReg, auto preIndReg, auto dmaBufferReg)
                                                     {
                                                         // Load group index and weight buffer from event source->process table
                                                         c.lw(*SStride, *eventSourceProcessOffsetReg, std::get<1>(eventTableLocations) + 8);
                                                         c.lw(*SWeightBuffer, *eventSourceProcessOffsetReg, std::get<1>(eventTableLocations) + 12);

                                                         {
                                                             ALLOCATE_SCALAR(STemp);
                                                             c.mul(*STemp, *preIndReg, *SStride);
                                                             c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
                                                         }
                                                         

                                                         // Start DMA write into RowBufferA
                                                         Assembler::Utils::generateDMAStartWrite(c, *dmaBufferReg, *SWeightBuffer, *SStride);
                                                     },
                                                     // Process event
                                                     [SGroupIndex, SMergedGroupReturn, SDMARowBufferA, SDMARowBufferB, SStride, SWeightBuffer, &eventTableLocations, &scalarRegisterAllocator]
                                                     (auto &c, auto eventSourceProcessOffsetReg, auto preIndReg)
                                                     {
                                                         ALLOCATE_SCALAR(SProcessAddress);

                                                         // Load stride, process address and group index
                                                         c.lw(*SStride, *eventSourceProcessOffsetReg, std::get<1>(eventTableLocations) + 8);
                                                         c.lw(*SProcessAddress, *eventSourceProcessOffsetReg, std::get<1>(eventTableLocations) + 16);
                                                         c.lw(*SGroupIndex, *eventSourceProcessOffsetReg, std::get<1>(eventTableLocations) + 20);
                                                         
                                                         // **TODO* relatively certain SWeightBuffer is ALWAYS dmaBuffer reg so can maybe just use this
                                                         c.mv(*SWeightBuffer, *eventSourceProcessOffsetReg, dmaBufferReg);

                                                         // Jump to process address
                                                         c.jalr(*SMergedGroupReturn, *SProcessAddress);
                                                     });*/
                                             }
                                             // Otherwise, generate event loop with weights in URAM
                                             else {
                                                 eventLoopScalarRegisterMask |= m.template getArchetype<EventSourceImplementation>()->generateURAMEventLoop(
                                                     m, *this, *ki, mergedEventSourceFields.first->second.back().second,
                                                     timeRegister, SPreIndReg, std::get<0>(eventTableLocations),
                                                     fieldBase, eventLoopCodeGenerator, scalarRegisterAllocator,
                                                     [SGroupIndex, SMergedGroupReturn, SPreIndReg, SStride, SWeightBuffer, &eventTableLocations, &scalarRegisterAllocator]
                                                     (auto &c, auto eventSourceProcessOffsetReg)
                                                     {
                                                        // Load process address, group index and weight buffer from event source->process table
                                                        ALLOCATE_SCALAR(SProcessAddress);
                                                        c.lw(*SProcessAddress, *eventSourceProcessOffsetReg, std::get<1>(eventTableLocations));
                                                        c.lw(*SGroupIndex, *eventSourceProcessOffsetReg, std::get<1>(eventTableLocations) + 4);
                                                        c.lw(*SStride, *eventSourceProcessOffsetReg, std::get<1>(eventTableLocations) + 8);
                                                        c.lw(*SWeightBuffer, *eventSourceProcessOffsetReg, std::get<1>(eventTableLocations) + 12);

                                                        {
                                                            ALLOCATE_SCALAR(STemp);
                                                            c.mul(*STemp, *SPreIndReg, *SStride);
                                                            c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
                                                        }
                                                        // Jump to process address
                                                        c.jalr(*SMergedGroupReturn, *SProcessAddress);
                                                     });
                                             }
                                         }

                                         LOGD_FENN_BACKEND << "Event loops require " << ::Common::Utils::popCount(eventLoopScalarRegisterMask) << " scalar registers";

                                         // Mask registers required for event loops
                                         scalarRegisterAllocator.maskRegisters(eventLoopScalarRegisterMask);

                                         // Generate blocks of code to update each merged event propagation process
                                         // These code blocks are event driven so expect SPreIndex, SGroupIndex 
                                         // and SMergedGroupReturn to be populated before jumping to them
                                         for(const auto &m : mergedProcesses) {
                                             // Ensure process has proper base class
                                             auto pi = std::dynamic_pointer_cast<const EventDrivenProcessImplementation>(m.getArchetype());
                                             if (!pi) {
                                                 throw std::runtime_error("FeNN backend runtime used with incompatible process");
                                             }
                                            
                                             // Define label
                                             c.L(mergedProcessLabels.at(m.getArchetype()));

                                             // Add new merged field
                                             // **NOTE** these are relative to start of field array
                                             mergedProcessGroupFields.first->second.emplace_back(std::piecewise_construct,
                                                                                                 std::make_tuple(fieldBase - fieldStart),
                                                                                                 std::make_tuple());

                                             // Generate code
                                             pi->generateCode(m, *this, *ki, mergedProcessGroupFields.first->second.back().second, 
                                                              timeRegister, SWeightBuffer, SStride, SGroupIndex, SPreIndReg, numTimesteps, 
                                                              fieldBase, c, scalarRegisterAllocator, vectorRegisterAllocator);
                                             
                                             // **HACK** add NOP to workaround issue with jumping to jump instructions
                                             // **NOTE** this is not ALWAYS required but it's safer to do it here 
                                             // than rely on each event propagation process correctly inserting it
                                             if(!generateSimulationKernels) {
                                                 c.nop();
                                             }

                                             // Return
                                             c.jr(*SMergedGroupReturn);
                                         }

                                         // Unmask event loop registers
                                         scalarRegisterAllocator.unmaskRegisters(eventLoopScalarRegisterMask);

                                         // End of kernel
                                         c.L(endProcessGroupLabel);

                                         // Add generted event loop code
                                         c += eventLoopCodeGenerator;

                                         // Resolve merged process label addresses 
                                         std::transform(mergedProcessLabels.cbegin(), mergedProcessLabels.cend(),
                                                        std::inserter(m_KernelMergedProcessAddresses[k], m_KernelMergedProcessAddresses[k].begin()),
                                                        [&c](const auto &m){ return std::make_pair(m.first, c.getAddress(m.second).value()); });
                                     }
                                     // Otherwise
                                     else {
                                        for(const auto &m : mergedProcesses) {
                                            // Ensure process has proper base class
                                            auto pi = std::dynamic_pointer_cast<const TimeDrivenProcessImplementation>(m.getArchetype());
                                            if (!pi) {
                                                throw std::runtime_error("FeNN backend runtime used with incompatible process");
                                            }

                                            // Add new merged field
                                            // **NOTE** these are relative to start of field array
                                            mergedProcessGroupFields.first->second.emplace_back(std::piecewise_construct,
                                                                                    std::make_tuple(fieldBase - fieldStart),
                                                                                    std::make_tuple());

                                            // Generate code
                                            pi->generateCode(m, *this, *ki, mergedProcessGroupFields.first->second.back().second, 
                                                             timeRegister, numTimesteps, fieldBase, c, 
                                                             scalarRegisterAllocator, vectorRegisterAllocator);
                                        }
                                     }
                                 });
            });

        // Add to kernel code dictionary
        m_KernelCode.try_emplace(k, code);
    }

    // Calculate number of bytes required for fields
    m_NumFieldBytes = fieldBase - fieldStart;
    LOGI_FENN_BACKEND << m_NumFieldBytes << " bytes of BRAM required for fields";
}
//----------------------------------------------------------------------------
void Runtime::allocatePreamble()
{
    // Loop through devices and allocate data structures
    for(auto &d : getDevices()) {
        static_cast<DeviceFeNN*>(d.get())->allocateDataStructures();
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

    // Loop through all devices 
    for(auto &d : getDevices()) {
        // Push field arrays to device
        static_cast<DeviceFeNN*>(d.get())->getFieldArray()->pushToDevice();

        // Populate data structures
        static_cast<DeviceFeNN*>(d.get())->populateDataStructures();
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
}
