#pragma once

// RISC-V assembler includes
#include "assembler/xbyak_riscv.hpp"

//----------------------------------------------------------------------------
// beq_01
//----------------------------------------------------------------------------
namespace ArchTest::beq_01
{
const std::vector<uint32_t> &getScalarInitData();

Xbyak_riscv::CodeGenerator generateCode();
}