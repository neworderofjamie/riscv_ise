#pragma once

// Standard C++ includes
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cstdint>

// Forward declarations
class ModelComponent;
class ProcessGroup;

//----------------------------------------------------------------------------
// Free functions
//----------------------------------------------------------------------------
std::vector<uint32_t> generateSimulationKernel(
    const ProcessGroup *synapseProcessGroup, const ProcessGroup *neuronProcessGroup,
    const std::unordered_map<const ModelComponent*, uint32_t> &uramAllocations,
    const std::unordered_map<const ModelComponent*, uint32_t> &bramAllocations,
    uint32_t numTimesteps, bool simulate);