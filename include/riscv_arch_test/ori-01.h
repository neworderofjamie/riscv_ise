#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// ori_01
//----------------------------------------------------------------------------
namespace ArchTest::ori_01
{
const std::vector<uint32_t> &getScalarInitData();

CodeGenerator generateCode();
}