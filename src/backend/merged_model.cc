#include "backend/merged_model.h"

// GeNN includes
#include "gennUtils.h"

// Model includes
#include "model/kernel.h"
#include "model/model.h"
#include "model/process.h"
#include "model/process_group.h"

//----------------------------------------------------------------------------
// Backend::MergedModel
//----------------------------------------------------------------------------
namespace Backend
{
MergedModel::MergedModel(const ::Model::Model &model)
{
    // Loop through all kernels in model
    for(const auto &k : model.getKernels()) {
        // Loop through all process groups in kernel
        const auto processGroups = k->getAllProcessGroups();
        for (const auto &g : processGroups) {
            // Create a hash map to group together processes with the same SHA1 digest
            std::unordered_map<boost::uuids::detail::sha1::digest_type, 
                               std::vector<std::shared_ptr<::Model::Process const>>, 
                               GeNN::Utils::SHA1Hash> protoMergedProcesses;
            
            // Add unmerged processes to correct vector
            for(const auto &p : g->getProcesses()) {
                // Build hash digest
                boost::uuids::detail::sha1 hash;
                p->updateMergeHash(hash, model);
                const auto digest = hash.get_digest();

                // Add to map
                protoMergedProcesses[digest].push_back(p);
            }

            // Reserve final merged groups vector
            auto &mergedProcessGroup = m_MergedProcessGroups[g];
            mergedProcessGroup.reserve(protoMergedProcesses.size());

            // Construct merged groups
            size_t i = 0;
            for(auto &p : protoMergedProcesses) {
                mergedProcessGroup.emplace_back(i++, p.second);
            }
        }
    }
}
}