#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// bgeu_01
//----------------------------------------------------------------------------
namespace ArchTest::bgeu_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}