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
#include "assembler/assembler_export.h"
#include "assembler/register_allocator.h"

//----------------------------------------------------------------------------
// AssemblerUtils
//----------------------------------------------------------------------------
//! Utility functions for generating useful bits of FeNN assembly language
//! Used both by compiler/FeNN backend and applications written in assembly
namespace AssemblerUtils
{

// Generate
ASSEMBLER_EXPORT void generateScalarVectorMemcpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                                 ScalarRegisterAllocator &scalarRegisterAllocator,
                                                 std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> scalarPtr, 
                                                 std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> vectorPtr, 
                                                 std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> numVectors);

ASSEMBLER_EXPORT void generateVectorScalarMemcpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                                 ScalarRegisterAllocator &scalarRegisterAllocator,
                                                 uint32_t vectorPtr, uint32_t scalarPtr, uint32_t numVectors);

ASSEMBLER_EXPORT void generateLaneLocalScalarMemcpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                                    ScalarRegisterAllocator &scalarRegisterAllocator,
                                                    uint32_t laneLocalPtr, uint32_t scalarPtr, uint32_t numVectors);

// Generate code to copy 64-bit performance counter value from pair of CSR registers to scalar memory
ASSEMBLER_EXPORT void generatePerformanceCountWrite(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                                                    CSR lowCSR, CSR highCSR, uint32_t scalarPtr);

// Generate an unrolled loop body
ASSEMBLER_EXPORT void unrollLoopBody(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                                     uint32_t numIterations, uint32_t maxUnroll, uint32_t iterationBytes,
                                     Reg testBufferReg, bool alwaysGenerateTail,
                                     std::function<void(CodeGenerator&, uint32_t, bool)> genBodyFn, 
                                     std::function<void(CodeGenerator&, uint32_t)> genTailFn);

// Generate an unrolled loop body for a vectorised loop
ASSEMBLER_EXPORT void unrollVectorLoopBody(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator, 
                                           uint32_t numIterations, uint32_t maxUnroll, Reg testBufferReg,
                                           std::function<void(CodeGenerator&, uint32_t, bool, ScalarRegisterAllocator::RegisterPtr)> genBodyFn, 
                                           std::function<void(CodeGenerator&, uint32_t)> genTailFn);

// Generate preamble and postamble for code using standard ecall instruction to terminate simulations and polling on device
ASSEMBLER_EXPORT std::vector<uint32_t> generateStandardKernel(bool simulate, uint32_t readyFlagPtr, 
                                                              std::function<void(CodeGenerator&, VectorRegisterAllocator&, ScalarRegisterAllocator&)> genBodyFn);


//! Generate code to calculate dest -= subtract in Uint64
ASSEMBLER_EXPORT void generateSubtractUint64(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                                             Reg destLow, Reg destHigh, Reg subtractLow, Reg subtractHigh);

//! Generate code to calculate dest += subtract in Uint64
ASSEMBLER_EXPORT void generateAddUint64(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                                        Reg destLow, Reg destHigh, Reg addLow, Reg addHigh);

//! Generate code to initiate a DMA write
ASSEMBLER_EXPORT void generateDMAStartWrite(CodeGenerator &c, Reg destination, Reg source, Reg size);

//! Generate code to initiate a DMA read
ASSEMBLER_EXPORT void generateDMAStartRead(CodeGenerator &c, Reg destination, Reg source, Reg size);

//! Generate code to (busy) wait until DMA write completes. Returns resulting status value
ASSEMBLER_EXPORT ScalarRegisterAllocator::RegisterPtr generateDMAWaitForWriteComplete(
    CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator);

//! Generate code to (busy) wait until DMA read completes. Returns resulting status value
ASSEMBLER_EXPORT ScalarRegisterAllocator::RegisterPtr generateDMAWaitForReadComplete(
    CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator);
}
