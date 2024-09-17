#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// sll_01
//----------------------------------------------------------------------------
namespace ArchTest::sll_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}