#include "fenn/backend/model.h"

// PLOG includes
#include <plog/Log.h>

// GeNN includes
#include "gennUtils.h"

// Common includes
#include "common/utils.h"

// FeNN backend includes
#include "fenn/backend/process.h"

//----------------------------------------------------------------------------
// FeNN::Backend::MemSpaceCompatibility
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
//----------------------------------------------------------------------------
// FeNN::Backend::Model
//----------------------------------------------------------------------------
Model::Model(const Model::KernelVector &graphs)
:   ::Model::Model(graphs)
{
    // Loop through all model state
    // **THINK** this kinda only need to be variables
    for (const auto &s : getStateProcesses()) {
        // Start with all memory spaces being compatible
        MemSpace compatibleMemSpaces = (MemSpace::LLM | MemSpace::URAM | MemSpace::BRAM
                                        | MemSpace::DRAM | MemSpace::URAM_LLM);

        // Loop through all processes using this state
        for (const auto &p : s.second) {
            // Ensure process has proper base class
            auto pi = std::dynamic_pointer_cast<const ProcessImplementation>(p);
            if (!pi) {
                throw std::runtime_error("FeNN backend model used with incompatible process");
            }

            // Update memory space compatiblity
            pi->updateMemSpaceCompatibility(s.first, compatibleMemSpaces);
        }


        // Count number of trailing zeros in compatible memory spaces 
        // i.e. how many are unsupported
        const int numTZ = ::Common::Utils::ctz(static_cast<uint32_t>(compatibleMemSpaces));

        // Obtain best memory space
        const MemSpace bestMemSpace = static_cast<MemSpace>(1 << numTZ);
        LOGI << "State object '" << s.first->getName() << "' compatible with " << static_cast<uint32_t>(compatibleMemSpaces) << " memory spaces, best memory space " << static_cast<uint32_t>(bestMemSpace);

        // Add best memory state for state to map
        m_StateMemSpace.try_emplace(s.first, bestMemSpace);
    }
}
}