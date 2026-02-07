#pragma once

// Standard C++ includes
#include <functional>
#include <variant>

// Standard C includes
#include <cstdint>

// RISC-V common includes
#include "fenn/common/isa.h"

// RISC-V assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/assembler_export.h"
#include "fenn/assembler/register_allocator.h"

//----------------------------------------------------------------------------
// FeNN::Assembler::Utils
//----------------------------------------------------------------------------
//! Utility functions for generating useful bits of FeNN assembly language
//! Used both by compiler/FeNN backend and applications written in assembly
namespace FeNN::Assembler::Utils
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

ASSEMBLER_EXPORT void generateScalarLaneLocalBroadcast(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                                       ScalarRegisterAllocator &scalarRegisterAllocator,
                                                       uint32_t scalarPtr, uint32_t laneLocalPtr, uint32_t numHalfWords);

// Generate code to copy 64-bit performance counter value from pair of CSR registers to scalar memory
ASSEMBLER_EXPORT void generatePerformanceCountWrite(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                                                    Common::CSR lowCSR, Common::CSR highCSR, uint32_t scalarPtr);

// Generate an unrolled loop body
ASSEMBLER_EXPORT void unrollLoopBody(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                                     uint32_t numIterations, uint32_t maxUnroll, uint32_t iterationBytes,
                                     Common::Reg testBufferReg, bool alwaysGenerateTail,
                                     std::function<void(CodeGenerator&, uint32_t, bool)> genBodyFn, 
                                     std::function<void(CodeGenerator&, uint32_t)> genTailFn);

// Generate an unrolled loop body for a vectorised loop
ASSEMBLER_EXPORT void unrollVectorLoopBody(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator, 
                                           uint32_t numIterations, uint32_t maxUnroll, Common::Reg testBufferReg,
                                           std::function<void(CodeGenerator&, uint32_t, bool, ScalarRegisterAllocator::RegisterPtr)> genBodyFn, 
                                           std::function<void(CodeGenerator&, uint32_t)> genTailFn);

// Generate preamble and postamble for code using standard ecall instruction to terminate simulations and polling on device
ASSEMBLER_EXPORT std::vector<uint32_t> generateStandardKernel(bool simulate, uint32_t readyFlagPtr, 
                                                              std::function<void(CodeGenerator&, VectorRegisterAllocator&, ScalarRegisterAllocator&)> genBodyFn);


//! Generate code to calculate dest -= subtract in Uint64
ASSEMBLER_EXPORT void generateSubtractUint64(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                                             Common::Reg destLow, Common::Reg destHigh, Common::Reg subtractLow, Common::Reg subtractHigh);

//! Generate code to calculate dest += subtract in Uint64
ASSEMBLER_EXPORT void generateAddUint64(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                                        Common::Reg destLow, Common::Reg destHigh, Common::Reg addLow, Common::Reg addHigh);

//! Generate code to initiate a DMA write
ASSEMBLER_EXPORT void generateDMAStartWrite(CodeGenerator &c, Common::Reg destination, Common::Reg source, Common::Reg size);

//! Generate code to initiate a DMA read
ASSEMBLER_EXPORT void generateDMAStartRead(CodeGenerator &c, Common::Reg destination, Common::Reg source, Common::Reg size);

//! Generate code to (busy) wait until DMA write completes. Returns resulting status value
ASSEMBLER_EXPORT ScalarRegisterAllocator::RegisterPtr generateDMAWaitForWriteComplete(
    CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator);

//! Generate code to (busy) wait until DMA read completes. Returns resulting status value
ASSEMBLER_EXPORT ScalarRegisterAllocator::RegisterPtr generateDMAWaitForReadComplete(
    CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator);

//! Generate code to (busy) wait on a router barrier
ASSEMBLER_EXPORT void generateRouterBarrier(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator, uint32_t numCores);
}   // namespace FeNN::Assembler::Utils
