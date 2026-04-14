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
}
