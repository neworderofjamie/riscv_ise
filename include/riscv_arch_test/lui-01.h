#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// lui_01
//----------------------------------------------------------------------------
namespace ArchTest::lui_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}