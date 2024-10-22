#pragma once

// RISC-V assembler includes
#include "assembler/assembler.h"

//----------------------------------------------------------------------------
// clz_01
//----------------------------------------------------------------------------
namespace ArchTest::clz_01
{
const std::vector<uint32_t> &getScalarInitData();

std::vector<uint32_t> generateCode(bool simulate);
}