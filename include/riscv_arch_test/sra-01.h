#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// sra_01
//----------------------------------------------------------------------------
namespace ArchTest::sra_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}