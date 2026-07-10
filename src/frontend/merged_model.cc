#include "frontend/merged_model.h"

// Common includes
#include "common/utils.h"

// Model includes
#include "frontend/kernel.h"
#include "frontend/model.h"
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
// Frontend::MergedModel
//----------------------------------------------------------------------------
MergedModel::MergedModel(const Model &model)
:   m_Model(model)
{
    // Loop through all kernels in model
    for(const auto &k : model.getKernels()) {
        // Loop through all process groups in kernel
        const auto processGroups = k->getAllProcessGroups();
        for (const auto &g : processGroups) {
            // Create merged process group for each process group
            if (!m_MergedProcessGroups.try_emplace(g, model, g).second) {
                throw std::runtime_error("Process group '" + g->getName() + "' referenced multiples time in model");
            }
        }
    }
}
}
