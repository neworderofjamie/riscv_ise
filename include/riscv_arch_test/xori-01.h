#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// xori_01
//----------------------------------------------------------------------------
namespace ArchTest::xori_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}