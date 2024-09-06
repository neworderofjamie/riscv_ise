#pragma once

// RISC-V assembler includes
#include "assembler/xbyak_riscv.hpp"

//----------------------------------------------------------------------------
// blt_01
//----------------------------------------------------------------------------
namespace ArchTest::blt_01
{
const std::vector<uint32_t> &getScalarInitData();

Xbyak_riscv::CodeGenerator generateCode();
}