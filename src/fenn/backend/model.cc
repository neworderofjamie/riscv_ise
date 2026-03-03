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
// FeNN::Backend::Model
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
Model::Model(const KernelVector &graphs)
:   Frontend::Model(graphs)
{
    // Loop through all model state
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
