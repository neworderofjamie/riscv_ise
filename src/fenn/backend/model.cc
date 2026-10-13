#include "fenn/backend/model.h"

// Standard C++ includes
#include <algorithm>
#include <iterator>
#include <sstream>

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/utils.h"

// Frontend includes
#include "frontend/process_group.h"

// FeNN backend includes
#include "fenn/backend/process.h"

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
const std::set<std::string> supportedPerformanceCounters{"num_instructions",
                                                         "num_clock_cycles"};
}

//----------------------------------------------------------------------------
// FeNN::Backend::Model
//----------------------------------------------------------------------------
namespace FeNN::Backend
{
Model::Model(const KernelVector &kernels)
:   Frontend::Model(kernels), m_PerformanceCounterArraySize(0)
{
    // Loop through all model state
    // **YUCK** we really only want to do this for variables
    for (const auto &s : getStateData()) {
        // Start with all memory spaces being compatible
        MemSpace compatibleMemSpaces = (MemSpace::LLM | MemSpace::URAM | MemSpace::BRAM | MemSpace::DRAM);

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

    // Loop through groups with performance counters 
    for(const auto &p : getPerformanceCounters()) {
        // If any performance counters are unsupported
        std::set<std::string> unsupportedPerformanceCounters;
        std::set_difference(p.second.cbegin(), p.second.cend(), 
                            supportedPerformanceCounters.cbegin(), supportedPerformanceCounters.cend(),
                            std::inserter(unsupportedPerformanceCounters, unsupportedPerformanceCounters.end()));
        if(!unsupportedPerformanceCounters.empty()) {
            std::ostringstream unsupportedPerformanceCountersString;
            std::copy(unsupportedPerformanceCounters.cbegin(), unsupportedPerformanceCounters.cend(),
                      std::ostream_iterator<std::string>(unsupportedPerformanceCountersString, ", "));
            throw std::runtime_error("Performance counters not supported by FeNN backend '" + unsupportedPerformanceCountersString.str() + "'");
        }

        // Add offset to array
        m_PerformanceCounterArrayOffsets.try_emplace(p.first, m_PerformanceCounterArraySize);
        m_PerformanceCounterArraySize += p.second.size();
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
//----------------------------------------------------------------------------
size_t Model::getPerformanceCounterArrayOffset(std::shared_ptr<const Frontend::ProcessGroup> processGroup) const
{
    return m_PerformanceCounterArrayOffsets.at(processGroup);
}
}
