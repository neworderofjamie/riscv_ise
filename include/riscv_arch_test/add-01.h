#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// add_01
//----------------------------------------------------------------------------
namespace ArchTest::add_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}