#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// sub_01
//----------------------------------------------------------------------------
namespace ArchTest::sub_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}