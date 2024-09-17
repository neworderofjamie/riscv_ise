#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// lw_align_01
//----------------------------------------------------------------------------
namespace ArchTest::lw_align_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}