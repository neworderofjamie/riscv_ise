#include "fenn/backend/kernel.h"

// Standard C++ includes
#include <numeric>
#include <set>

// Common include
#include "common/utils.h"

// Frontend include
#include "frontend/process_group.h"

// FeNN assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/assembler_utils.h"
#include "fenn/assembler/register_allocator.h"

// FeNN backend includes
#include "fenn/backend/events.h"
#include "fenn/backend/process.h"

using namespace FeNN;

//----------------------------------------------------------------------------
// FeNN::Backend::KernelImplementation
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
KernelImplementation::KernelImplementation(const Frontend::ProcessGroupVector &processGroups)
:   m_NumNeuronIDBits(0), m_NumPopulationIDBits(0)
{
    // Loop through all process groups in kernel
    // **NOTE** at least 5 bits need to be used for neuron ID
    std::unordered_map<std::shared_ptr<const Frontend::EventSink>, std::optional<uint32_t>> eventSinkIDs;
    std::set<uint32_t> allocatedPopulationIDs;
    m_NumNeuronIDBits = 5;
    for (const auto &g : processGroups) {
        // Loop through processes in group
        bool allEvent = true;
        bool noEvent = true;
        for (const auto &p : g->getProcesses()) {
            // If this process doesn't have any event sources, then this group can't contain all event sources
            // **TODO** is getEventSource() enough?
            const auto eventSources = p->getAllEventSources();
            if(eventSources.empty()) {
                allEvent = false; 
            }
            // Otherwise
            else {
                // Flag that this group can't contain event sources
                noEvent = false;

                // Loop through event sources
                for(const auto &e : eventSources) {
                    // Add mapping between event source and process
                    m_EventSourceProcesses[e].push_back(p);

                    // If this source is the output of an event channel
                    auto eventSourceChannel = std::dynamic_pointer_cast<const Frontend::EventChannelSource>(e);
                    if(eventSourceChannel) {
                        // If the sink at the other end requires a routing key
                        auto eventSink = eventSourceChannel->getSink();
                        auto eventSinkRouter = std::dynamic_pointer_cast<const EventSinkRouterKeyImplementation>(eventSink);
                        if(eventSinkRouter) {
                            // If event sink specified a hard-coded population ID 
                            const auto populationID = eventSinkRouter->getPopulationID();
                            if(populationID) {
                                // If population ID isn't a multiple of four give error
                                // **NOTE** this is a micro-optimisation to save an instruction when processing events - we are going to have 2 bits spare for a while!
                                if((populationID.value() % 4) != 0) {
                                    throw std::runtime_error("Event sink '" + eventSink->getName() 
                                                             + "' requesting population ID: "
                                                             + std::to_string(populationID.value())
                                                             + " which is not a multiple of 4");
                                }

                                // Add to set, giving error if this ID has already been allocated
                                if(!allocatedPopulationIDs.emplace(populationID.value()).second) {
                                    throw std::runtime_error("Event sink '" + eventSink->getName() 
                                                             + "' requesting duplicate population ID: "
                                                             + std::to_string(populationID.value()));
                                }
                            }

                            // Add event sink and hard-coded population ID (if any) to map
                            eventSinkIDs.try_emplace(eventSink, populationID);

                            // Update maximum number of neuron ID bits
                            m_NumNeuronIDBits = std::max(m_NumNeuronIDBits, 
                                                         eventSinkRouter->getNumNeuronIDBits());
                        }
                    }
                }
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

    // If there are any event sinks
    if(!eventSinkIDs.empty()) {
        uint32_t nextID = 0;
        std::transform(eventSinkIDs.cbegin(), eventSinkIDs.cend(),
                       std::inserter(m_EventSinkIDs, m_EventSinkIDs.begin()),
                       [&allocatedPopulationIDs, &nextID](const auto &e)
                       {
                           // If population ID is hardcoded, add directly
                           if(e.second) {
                               LOGD_FENN_BACKEND << "Event sink '" << e.first->getName() << "' using hard-coded population ID: " << e.second.value();
                               return std::make_pair(e.first, e.second.value());
                           } 
                           // Otherwise
                           else {
                               // Advance while next ID has already been allocated
                               while(allocatedPopulationIDs.find(nextID) != allocatedPopulationIDs.end()) {
                                   nextID += 4;
                               }

                               LOGD_FENN_BACKEND << "Event sink '" << e.first->getName() << "' allocated population ID: " << nextID;
                               auto result = std::make_pair(e.first, nextID);
                               nextID += 4;
                               return result;
                           }
                       });

        // Get largest population ID
        const uint32_t maxPopulationID = (allocatedPopulationIDs.empty() 
                                          ? nextID 
                                          : std::max(nextID, *allocatedPopulationIDs.rbegin()));

        // Count bits required to represent largest population index
        m_NumPopulationIDBits = ::Common::Utils::getNumBits(maxPopulationID);
        LOGI_FENN_BACKEND << "Neuron IDs require " << m_NumNeuronIDBits << " and population IDs require " << m_NumPopulationIDBits << " bits";
        if ((m_NumNeuronIDBits + m_NumPopulationIDBits) > 24) {
            throw std::runtime_error("Insufficient event address space");
        }
    }
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
    // Visit process groups
    for (const auto &p : getProcessGroups()) {
        generateProcessGroup(p, nullptr, std::nullopt, c, 
                             scalarRegisterAllocator, vectorRegisterAllocator);
    }
}
//----------------------------------------------------------------------------
bool SimpleKernel::requiresPerformanceCounters() const
{
    // Performance counters are required if any process groups want to record performance
    return std::any_of(getProcessGroups().cbegin(), getProcessGroups().cend(),
                       [](const auto &g){ return g->shouldRecordPerformance(); });
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
//----------------------------------------------------------------------------
bool SimulationLoopKernel::requiresPerformanceCounters() const
{
    // Performance counters are required if any of the process groups want to record performance
    // **TODO** realtime!
    return (std::any_of(getBeginProcessGroups().cbegin(), getBeginProcessGroups().cend(),
                        [](const auto &g){ return g->shouldRecordPerformance(); })
            || std::any_of(getTimestepProcessGroups().cbegin(), getTimestepProcessGroups().cend(),
                        [](const auto &g){ return g->shouldRecordPerformance(); })
            || std::any_of(getEndProcessGroups().cbegin(), getEndProcessGroups().cend(),
                        [](const auto &g){ return g->shouldRecordPerformance(); }));
}
}
