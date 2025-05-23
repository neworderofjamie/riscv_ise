#pragma once

// Standard C++ includes
#include <functional>
#include <variant>

// Standard C includes
#include <cstdint>

// RISC-V common includes
#include "common/isa.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/register_allocator.h"

//----------------------------------------------------------------------------
// AssemblerUtils
//----------------------------------------------------------------------------
namespace AssemblerUtils
{

// Generate
void generateScalarVectorMemcpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                ScalarRegisterAllocator &scalarRegisterAllocator,
                                std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> scalarPtr, 
                                std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> vectorPtr, 
                                std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> numVectors);

void generateVectorScalarMemcpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                ScalarRegisterAllocator &scalarRegisterAllocator,
                                uint32_t vectorPtr, uint32_t scalarPtr, uint32_t numVectors);

// Generate code to copy 64-bit performance counter value from pair of CSR registers to scalar memory
void generatePerformanceCountWrite(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                                   CSR lowCSR, CSR highCSR, uint32_t scalarPtr);

// Generate an unrolled loop body
void unrollLoopBody(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                    uint32_t numIterations, uint32_t maxUnroll, uint32_t iterationBytes,
                    Reg testBufferReg, bool alwaysGenerateTail,
                    std::function<void(CodeGenerator&, uint32_t, bool)> genBodyFn, 
                    std::function<void(CodeGenerator&, uint32_t)> genTailFn);

// Generate an unrolled loop body for a vectorised loop
void unrollVectorLoopBody(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator, 
                          uint32_t numIterations, uint32_t maxUnroll, Reg testBufferReg,
                          std::function<void(CodeGenerator&, uint32_t, bool, ScalarRegisterAllocator::RegisterPtr)> genBodyFn, 
                          std::function<void(CodeGenerator&, uint32_t)> genTailFn);

// Generate preamble and postamble for code using standard ecall instruction to terminate simulations and polling on device
std::vector<uint32_t> generateStandardKernel(bool simulate, uint32_t readyFlagPtr, 
                                             std::function<void(CodeGenerator&, VectorRegisterAllocator&, ScalarRegisterAllocator&)> genBodyFn);
}
