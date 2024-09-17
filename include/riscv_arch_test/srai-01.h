#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// srai_01
//----------------------------------------------------------------------------
namespace ArchTest::srai_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}