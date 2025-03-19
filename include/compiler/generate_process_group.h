#pragma once

// Standard C++ includes
#include <memory>
#include <unordered_map>
#include <vector>

// Standard C includes
#include <cstdint>

// Compiler includes
#include "compiler/process_fields.h"

// Forward declarations
class ModelComponent;
class ProcessGroup;

//----------------------------------------------------------------------------
// Free functions
//----------------------------------------------------------------------------
std::vector<uint32_t> generateSimulationKernel(std::shared_ptr<const ProcessGroup> synapseProcessGroup, 
                                               std::shared_ptr<const ProcessGroup> neuronProcessGroup,
                                               const ProcessFields &processFields,
                                               uint32_t numTimesteps, bool simulate);