#include "fenn/backend/kernel.h"

// Common include
#include "common/utils.h"

// Frontend include
#include "frontend/process_group.h"

// FeNN common includes
#include "fenn/common/isa.h"

// FeNN assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/register_allocator.h"

// FeNN backend includes
#include "fenn/backend/process.h"

using namespace FeNN;

//----------------------------------------------------------------------------
// FeNN::Backend::SimpleGraph
//----------------------------------------------------------------------------
namespace
{
bool arePerformanceCountersRequired(const Frontend::ProcessGroupVector &processGroups)
{
    return std::any_of(processGroups.cbegin(), processGroups.cend(),
                       [](const auto &p){ return p->shouldRecordPerformance(); });
}
}

//----------------------------------------------------------------------------
// FeNN::Backend::KernelImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
KernelImplementation::KernelImplementation(const Frontend::ProcessGroupVector &processGroups)
{
    // Loop through kernels
    size_t maxEventSinkSize = 0;

    // Loop through all process groups in kernel
    for (const auto &g : processGroups) {
        // Loop through processes in group
        bool allEvent = true;
        bool noEvent = true;
        for (const auto &p : g->getProcesses()) {
            // If this process doesn't have any event sources, then this group can't contain all event sources
            // **TODO** is getEventSource() enough?
            if(p->getAllEventSources().empty()) {
                allEvent = false; 
            }
            // Otherwise, this group can't contain NO event sources
            else {
                noEvent = false;
            }

            // Loop through event sinks associated with this process, update count and maximum event sink size
            // **NOTE** events are broadcast so no need to consider core split here
            const auto eventSinks = p->getAllEventSinks();
            for(const auto &e : eventSinks) {
                // Allocate event sink ID and add to mape
                if(!m_EventSinkIDs.try_emplace(e.getUnderlying(), m_EventSinkIDs.size()).second) {
                    throw std::runtime_error("Duplicate event sinks encountered in model");
                }

                // Update maximum size
                maxEventSinkSize = std::max(maxEventSinkSize, e.getShape().getFlattenedSize());
            }
        }

        // If group has a mixture of processes with and without event sources or another process group with event sources has already been found
        if ((!allEvent && !noEvent) || (allEvent && m_EventSourceProcessGroup)) {
            throw std::runtime_error("On FeNN, all event sources need to be in a single process group");
        }
        // Otherwise, if process group 
        else if(allEvent) {
            m_EventSourceProcessGroup = g;
        }
    }

    // If there are no event sinks
    if(m_EventSinkIDs.empty()) {
        LOGI_FENN_BACKEND << "No event sinks found";
    }
    // Otherwise
    else {
        // Count bits required to represent largest neuron and population index
        m_NumNeuronIDBits = 32 - ::Common::Utils::clz(maxEventSinkSize - 1);
        m_NumPopulationIDBits = 32 - ::Common::Utils::clz(m_EventSinkIDs.size() - 1);
        LOGI_FENN_BACKEND << "Neuron IDs require " << m_NumNeuronIDBits << " and population IDs require " << m_NumPopulationIDBits << " bits";
        if ((m_NumNeuronIDBits + m_NumPopulationIDBits) > 24) {
            throw std::runtime_error("Insufficient event address space");
        }
    }

    if(m_EventSourceProcessGroup) {
        LOGI_FENN_BACKEND << "Event source process group found: '" << m_EventSourceProcessGroup->getName();
    }
    else {
        LOGI_FENN_BACKEND << "No event source process group found";
    }
    // **TODO** merge the event sources in
}
//----------------------------------------------------------------------------
uint32_t KernelImplementation::getEventSinkIDBase(std::shared_ptr<const Frontend::EventSink> eventSink) const
{
    // Get ID of this event sink
    const uint32_t id = m_EventSinkIDs.at(eventSink);

    // Shift up by the number of neuron ID bitss
    return (id << m_NumNeuronIDBits);
}

//----------------------------------------------------------------------------
// FeNN::Backend::SimpleKernel
//----------------------------------------------------------------------------
void SimpleKernel::generateCode(Assembler::CodeGenerator &c,
                                Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                GenerateProcessGroupFn generateProcessGroup) const
{
    // If performance counters are enabled, disinhibit them
    // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
    if(arePerformanceCountersRequired(getProcessGroups())) {
        c.csrw(Common::CSR::MCOUNTINHIBIT, Common::Reg::X0);
    }

    // Visit process groups
    for (const auto &p : getProcessGroups()) {
        generateProcessGroup(p, nullptr, std::nullopt, c, 
                             scalarRegisterAllocator, vectorRegisterAllocator);
    }
}
//----------------------------------------------------------------------------
// FeNN::Backend::SimulationLoopKernel
//----------------------------------------------------------------------------
void SimulationLoopKernel::generateCode(Assembler::CodeGenerator &c,
                                        Assembler::ScalarRegisterAllocator &scalarRegisterAllocator, 
                                        Assembler::VectorRegisterAllocator &vectorRegisterAllocator,
                                        GenerateProcessGroupFn generateProcessGroup) const
{
    // Register allocation
    ALLOCATE_SCALAR(STime);
    ALLOCATE_SCALAR(STimeEnd);

    // If performance counters are enabled, disinhibit them
    // **NOTE** on device, this takes a few cycles to make it through the pipeline so we do it well before we try and access counters
    if(arePerformanceCountersRequired(getTimestepProcessGroups()) 
       || arePerformanceCountersRequired(getBeginProcessGroups())
       || arePerformanceCountersRequired(getEndProcessGroups())) 
    {
        c.csrw(Common::CSR::MCOUNTINHIBIT, Common::Reg::X0);
    }

    // Set timestep range and load ready flag pointer
    c.li(*STime, 0);
    c.li(*STimeEnd, getNumTimesteps());

    // Visit begin process group
    for (const auto &p : getBeginProcessGroups()) {
        generateProcessGroup(p, nullptr, std::nullopt, c,
                             scalarRegisterAllocator, vectorRegisterAllocator);
    }

    // Loop over time
    auto timeLoop = c.L();
    {
        // Visit timestep process group
        for (const auto &p : getTimestepProcessGroups()) {
            generateProcessGroup(p, STime, getNumTimesteps(), c, 
                                 scalarRegisterAllocator, vectorRegisterAllocator);
        }

        c.addi(*STime, *STime, 1);
        c.bne(*STime, *STimeEnd, timeLoop);
    }

    // Visit end process group
    for (const auto &p : getEndProcessGroups()) {
        generateProcessGroup(p, nullptr, std::nullopt, c, 
                             scalarRegisterAllocator, vectorRegisterAllocator);
    }
}

}
