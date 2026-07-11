#include "frontend/model.h"

// Common includes
#include "common/utils.h"

// Model includes
#include "frontend/kernel.h"
#include "frontend/process.h"
#include "frontend/process_group.h"

//----------------------------------------------------------------------------
// Frontend::MergedProcessGroup
//----------------------------------------------------------------------------
namespace Frontend
{
MergedProcessGroup::MergedProcessGroup(const Model &model, std::shared_ptr<ProcessGroup const> processGroup)
{
    // Create a hash map to group together processes with the same SHA1 digest
    std::unordered_map<boost::uuids::detail::sha1::digest_type, 
        std::vector<std::shared_ptr<Process const>>, 
        Common::Utils::SHA1Hash> protoMergedProcesses;

    // Add unmerged processes to correct vector
    for(const auto &p : processGroup->getProcesses()) {
        // Build hash digest
        boost::uuids::detail::sha1 hash;
        p->updateMergeHash(hash, model);
        const auto digest = hash.get_digest();

        // Add to map
        protoMergedProcesses[digest].push_back(p);
    }

    // Reserve final merged groups vector
    m_MergedProcesses.reserve(protoMergedProcesses.size());

    // Construct merged groups
    size_t i = 0;
    for(auto &p : protoMergedProcesses) {
        m_MergedProcesses.emplace_back(i++, p.second);

        // Add all processes in merged group to reverse lookup structure
        for(size_t j = 0; j < p.second.size(); j++) {
            const auto res = m_Destinations.try_emplace(p.second[j], 
                                                        m_MergedProcesses.back().getArchetype(), 
                                                        j);
            if(!res.second) {
                throw std::runtime_error("Process '" + p.second[j]->getName() + "' included multiple times in process group");
            }
        }
    }
}

//----------------------------------------------------------------------------
// Frontend::Model
//----------------------------------------------------------------------------
Model::Model(const KernelVector &kernels)
:   m_Kernels(kernels)
{
    // Loop through kernels
    for(const auto &k : getKernels()) {
        // Loop through all process groups in kernel
        const auto processGroups = k->getAllProcessGroups();
        for (const auto &g : processGroups) {
            // Loop through processes in group
            for (const auto &p : g->getProcesses()) {
                // Loop through all state associated with this process
                for (const auto &s : p->getAllState()) {
                    m_StateData[s].processes.push_back(p);
                }
            }

            // Create merged process group for each process group
            if (!m_MergedProcessGroups.try_emplace(g, *this, g).second) {
                throw std::runtime_error("Process group '" + g->getName() + "' referenced multiples time in model");
            }
        }
    }

    // Loop through all model state
    std::unordered_map<std::shared_ptr<const Frontend::State>, uint32_t> compatibleSplitDimensions;
    for (auto &s : m_StateData) {
        // Get dimensionality of state's shape
        const size_t numDims = s.first->getShape().getNumDims();
        if(numDims > 32) {
            throw std::runtime_error("State '" + s.first->getName() + "' has a shape with more than 32 dimensions");
        }

        // Start with all memory spaces being compatible
        uint32_t stateCompatibleSplitDimensions = (1 << numDims) - 1;

        // Loop through all processes using this state and update this compatibility
        for (const auto &p : s.second.processes) {
            p->updateCompatibleSplitDimensions(s.first, stateCompatibleSplitDimensions);
        }
        
        // Add to map
        compatibleSplitDimensions.try_emplace(s.first, stateCompatibleSplitDimensions);
    }

    // Loop through kernels
    for(const auto &k : getKernels()) {
        // Loop through all process groups in kernel
        const auto processGroups = k->getAllProcessGroups();
        for (const auto &g : processGroups) {
            // Loop through processes in group and allow them to constrain split dimensions
            for (const auto &p : g->getProcesses()) {
                p->constrainSplitDimensions(compatibleSplitDimensions);
            }
        }
    }

    // Loop through all model state
    for (auto &s : m_StateData) {
        // If this state cannot be spli
        const uint32_t stateCompatibleSplitDimensions = compatibleSplitDimensions.at(s.first);
        if(stateCompatibleSplitDimensions == 0) {
            s.second.splitDimension = std::nullopt;
        }
        // Otherwise, count leading-zeros to pick highest compatible dimension
        // to split on (this maximises the number of contiguous sections of data)
        else {
            s.second.splitDimension = 31 - ::Common::Utils::clz(stateCompatibleSplitDimensions);
        }
    }
}
//----------------------------------------------------------------------------
const Model::StateData &Model::getStateData(std::shared_ptr<const Frontend::State> state) const
{
    return m_StateData.at(state);
}
}