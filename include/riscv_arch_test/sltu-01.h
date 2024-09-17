#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// sltu_01
//----------------------------------------------------------------------------
namespace ArchTest::sltu_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}