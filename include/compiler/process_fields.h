#pragma once

// Standard C++ includes
#include <memory>
#include <unordered_map>

// Standard C includes
#include <cstdint>

// Forward declarations
class BRAMAllocator;
class Process;
class ProcessGroup;
class State;

// Mapping of state objects to address of field used to hold their address
// **TODO** eventually these will probably be offsets
using StateFields = std::unordered_map<std::shared_ptr<const State>, uint32_t>;

// Mapping of processes to their fields
using ProcessFields = std::unordered_map<std::shared_ptr<const Process>, StateFields>;

// Update process fields data structure from process group
void addFields(std::shared_ptr<const ProcessGroup> processGroup, BRAMAllocator &bramAllocator, ProcessFields &fields);
