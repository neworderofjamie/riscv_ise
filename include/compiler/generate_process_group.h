#pragma once

// Standard C++ includes
#include <vector>

// Standard C includes
#include <cstdint>

// Forward declarations
class ProcessGroup;

//----------------------------------------------------------------------------
// Free functions
//----------------------------------------------------------------------------
std::vector<uint32_t> generateSimulationKernel(const ProcessGroup *synapseProcessGroup, 
                                               const ProcessGroup *neuronProcessGroup,
                                               uint32_t numTimesteps, bool simulate);