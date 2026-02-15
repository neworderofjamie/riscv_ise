#include "fenn/backend/model.h"

//----------------------------------------------------------------------------
// FeNN::Backend::Model
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
Model::Model(const Model::GraphVector &graphs)
:   ::Model::Model(graphs)
{
    // Loop through all model state
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