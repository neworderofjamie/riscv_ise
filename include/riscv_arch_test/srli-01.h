#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// srli_01
//----------------------------------------------------------------------------
namespace ArchTest::srli_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}