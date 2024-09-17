#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// xor_01
//----------------------------------------------------------------------------
namespace ArchTest::xor_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}