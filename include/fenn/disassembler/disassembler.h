#pragma once

// Standard C++ includes
#include <sstream>

// Standard C includes
#include <cstdint>

// Disassembler includes
#include "disassembler/disassembler_export.h"

DISASSEMBLER_EXPORT void disassemble(std::ostream &os, uint32_t inst);