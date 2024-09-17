#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// addi_01
//----------------------------------------------------------------------------
namespace ArchTest::addi_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}