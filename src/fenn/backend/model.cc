#include "fenn/backend/model.h"

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/utils.h"

// Frontend includes
#include "frontend/process_group.h"

// FeNN backend includes
#include "fenn/backend/kernel.h"
#include "fenn/backend/process.h"

//----------------------------------------------------------------------------
// FeNN::Backend::Model
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
Model::Model(const KernelVector &kernels)
:   Frontend::Model(kernels)
{
    // Loop through all model state
    // **YUCK** we really only want to do this for variables
    for (const auto &s : getStateData()) {
        // Start with all memory spaces being compatible
        MemSpace compatibleMemSpaces = (MemSpace::LLM | MemSpace::URAM | MemSpace::BRAM
                                        | MemSpace::DRAM | MemSpace::URAM_LLM);

        // Loop through all processes using this state
        for (const auto &p : s.second.processes) {
            // Ensure process has proper base class
            auto pi = std::dynamic_pointer_cast<const ProcessImplementation>(p);
            if (!pi) {
                throw std::runtime_error("FeNN backend model used with incompatible process");
            }

            // Update memory space compatiblity
            pi->updateCompatibleMemSpace(s.first, compatibleMemSpaces);
        }

        // Add compatible memory states to map
        // **NOTE** cannot resolve best right now as e.g. whether
        // we want to use DRAM for weights is not currently known
        m_StateCompatibleMemSpaces.try_emplace(s.first, compatibleMemSpaces);
    }

    // Loop through kernels
    size_t maxEventSinkSize = 0;
    for (const auto &k : getKernels()) {
        // Loop through all process groups in kernel
        const auto processGroups = k->getAllProcessGroups();
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
MemSpace Model::getStateMemSpace(std::shared_ptr<const Frontend::State> state, bool useDRAMForWeights) const
{
    // Get compatible memory spaces
    auto compatibleMemSpaces = static_cast<uint32_t>(m_StateCompatibleMemSpaces.at(state));

    // If we shouldn't use DRAM, clear that bit
    if (!useDRAMForWeights) {
        compatibleMemSpaces &= ~static_cast<uint32_t>(MemSpace::DRAM);
    }

    // Count number of trailing zeros in compatible memory spaces 
    // i.e. how many are unsupported
    const int numTZ = ::Common::Utils::ctz(compatibleMemSpaces);

    // Obtain best memory space
    return static_cast<MemSpace>(1 << numTZ);
}
//----------------------------------------------------------------------------
uint32_t Model::getEventSinkIDBase(std::shared_ptr<const Frontend::EventSink> eventSink) const
{
    // Get ID of this event sink
    const uint32_t id = m_EventSinkIDs.at(eventSink);

    // Shift up by the number of neuron ID bitss
    return (id << m_NumNeuronIDBits);
}
}
