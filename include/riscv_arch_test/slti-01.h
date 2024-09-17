#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// slti_01
//----------------------------------------------------------------------------
namespace ArchTest::slti_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}