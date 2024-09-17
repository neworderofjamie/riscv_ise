#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// andi_01
//----------------------------------------------------------------------------
namespace ArchTest::andi_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}