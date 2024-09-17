#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// bge_01
//----------------------------------------------------------------------------
namespace ArchTest::bge_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}