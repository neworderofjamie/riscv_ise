#pragma once

// RISC-V assembler includes
#include "assembler/xbyak_riscv.hpp"

//----------------------------------------------------------------------------
// bgeu_01
//----------------------------------------------------------------------------
namespace ArchTest::bgeu_01
{
const std::vector<uint32_t> &getScalarInitData();

Xbyak_riscv::CodeGenerator generateCode();
}