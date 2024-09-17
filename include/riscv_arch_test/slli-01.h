#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// slli_01
//----------------------------------------------------------------------------
namespace ArchTest::slli_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}