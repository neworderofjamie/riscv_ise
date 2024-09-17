#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// blt_01
//----------------------------------------------------------------------------
namespace ArchTest::blt_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}