#pragma once

// RISC-V assembler includes
#include "assembler/xbyak_riscv.hpp"

//----------------------------------------------------------------------------
// bge_01
//----------------------------------------------------------------------------
namespace ArchTest::bge_01
{
const std::vector<uint32_t> &getScalarInitData();

Xbyak_riscv::CodeGenerator generateCode();
}