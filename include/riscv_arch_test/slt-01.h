#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// slt_01
//----------------------------------------------------------------------------
namespace ArchTest::slt_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}