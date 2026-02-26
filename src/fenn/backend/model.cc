#include "fenn/backend/model.h"

// GeNN includes
#include "gennUtils.h"

// FeNN backend includes
#include "fenn/backend/process.h"

//----------------------------------------------------------------------------
// FeNN::Backend::MemSpaceCompatibility
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
void MemSpaceCompatibility::updateHash(boost::uuids::detail::sha1 &hash) const
{
    GeNN::Utils::updateHash(bram, hash);
    GeNN::Utils::updateHash(dram, hash);
    GeNN::Utils::updateHash(llm, hash);
    GeNN::Utils::updateHash(uram, hash);
    GeNN::Utils::updateHash(uramLLM, hash);
}

//----------------------------------------------------------------------------
// FeNN::Backend::Model
//----------------------------------------------------------------------------
Model::Model(const Model::KernelVector &graphs)
:   ::Model::Model(graphs)
{
    // Loop through all model state
    // **THINK** this kinda only need to be variables
    for (const auto &s : getStateProcesses()) {
        // Loop through all processes using this state
        MemSpaceCompatibility m;
        for (const auto &p : s.second) {
            // Ensure process has proper base class
            auto pi = std::dynamic_pointer_cast<const ProcessImplementation>(p);
            if (!pi) {
                throw std::runtime_error("FeNN backend model used with incompatible process");
            }

            // Update memory space compatiblity
            pi->updateMemSpaceCompatibility(s.first, m);
        }

        // Add memory compatibility of state to map
        m_StateMemSpaceCompatibility.try_emplace(s.first, m);
    }
}
}