#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// sltiu_01
//----------------------------------------------------------------------------
namespace ArchTest::sltiu_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}