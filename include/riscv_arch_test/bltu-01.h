#pragma once

// RISC-V assembler includes
#include "assembler/xbyak_riscv.hpp"

//----------------------------------------------------------------------------
// bltu_01
//----------------------------------------------------------------------------
namespace ArchTest::bltu_01
{
const std::vector<uint32_t> &getScalarInitData();

Xbyak_riscv::CodeGenerator generateCode();
}