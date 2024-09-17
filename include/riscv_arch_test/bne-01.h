#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// bne_01
//----------------------------------------------------------------------------
namespace ArchTest::bne_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}