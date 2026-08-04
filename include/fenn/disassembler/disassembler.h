#pragma once

// Standard C++ includes
#include <sstream>

// Standard C includes
#include <cstdint>

// Disassembler includes
#include "fenn/disassembler/disassembler_export.h"

//----------------------------------------------------------------------------
// FeNN::Disassembler
//----------------------------------------------------------------------------
namespace FeNN::Disassembler
{
FENN_DISASSEMBLER_EXPORT void disassemble(std::ostream& os, uint32_t inst);
}