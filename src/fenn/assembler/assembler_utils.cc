#include "fenn/assembler/assembler_utils.h"

// Common includes
#include "common/utils.h"

using namespace FeNN;
using namespace FeNN::Assembler;

//----------------------------------------------------------------------------
// FeNN::Assembler::Utils
//----------------------------------------------------------------------------
namespace FeNN::Assembler::Utils
{
void generateScalarVectorMemcpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                ScalarRegisterAllocator &scalarRegisterAllocator,
                                std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> scalarPtr, 
                                std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> vectorPtr, 
                                std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> numVectors)
{
    // Register allocation
    ALLOCATE_SCALAR(SVectorBufferEnd);
    ALLOCATE_SCALAR(SOne);

    // Labels
    auto vectorLoop = createLabel();

    // If literal is provided for scalar pointer, allocate register and load immediate into it
    ScalarRegisterAllocator::RegisterPtr SDataBuffer;
    if(std::holds_alternative<uint32_t>(scalarPtr)) {
        SDataBuffer = scalarRegisterAllocator.getRegister("SDataBuffer = X");
        c.li(*SDataBuffer, std::get<uint32_t>(scalarPtr));
    }
    // Otherwise, use pointer register directly
    else {
        SDataBuffer = std::get<ScalarRegisterAllocator::RegisterPtr>(scalarPtr);
    }

    // If literal is provided for scalar pointer, allocate register and load immediate into it
    ScalarRegisterAllocator::RegisterPtr SVectorBuffer;
    if(std::holds_alternative<uint32_t>(vectorPtr)) {
        SVectorBuffer = scalarRegisterAllocator.getRegister("SVectorBuffer = X");
        c.li(*SVectorBuffer, std::get<uint32_t>(vectorPtr));
    }
    // Otherwise, use pointer register directly
    else {
        SVectorBuffer = std::get<ScalarRegisterAllocator::RegisterPtr>(vectorPtr);
    }

    // If literal is provided for number of vectors, load immediate and immediate to address
    if(std::holds_alternative<uint32_t>(numVectors)) {
        ALLOCATE_SCALAR(STmp);
        c.li(*STmp, std::get<uint32_t>(numVectors) * 64);
        c.add(*SVectorBufferEnd, *SVectorBuffer, *STmp);
    }
    // Otherwise, add register to address
    else {
        ALLOCATE_SCALAR(STmp);
        c.slli(*STmp, *std::get<ScalarRegisterAllocator::RegisterPtr>(numVectors), 6);
        c.add(*SVectorBufferEnd, *SVectorBuffer, *STmp);
    }

    // Load immedate
    c.li(*SOne, 1);

    // Loop over vectors
    c.L(vectorLoop);
    {
        // Register allocation
        ALLOCATE_VECTOR(VData);
        ALLOCATE_VECTOR(VLane);
        ALLOCATE_SCALAR(SMask);
        ALLOCATE_SCALAR(SVal);
        
        // Unroll lane loop
        for(int l = 0; l < 32; l++) {
            // Load halfword
            c.lh(*SVal, *SDataBuffer, l * 2);

            // SMask = 1 << SLane
            c.slli(*SMask, *SOne, l);

            // Fill vector register
            c.vfill(*VLane, *SVal);

            // VData = SMask ? VLane : VData
            c.vsel(*VData, *SMask, *VLane); 
        }

        // SDataBuffer += 64
        c.addi(*SDataBuffer, *SDataBuffer, 64);
      
        // *SVectorBuffer = VData
        c.vstore(*VData, *SVectorBuffer);

        // SVector += 64
        c.addi(*SVectorBuffer, *SVectorBuffer, 64);

        // If SVectorBuffer != SVectorBufferEnd, goto vector loop
        c.bne(*SVectorBuffer, *SVectorBufferEnd, vectorLoop);
    }
}
//----------------------------------------------------------------------------
void generateVectorScalarMemcpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                ScalarRegisterAllocator &scalarRegisterAllocator,
                                uint32_t vectorPtr, uint32_t scalarPtr, uint32_t numVectors)
{
    // Register allocation
    ALLOCATE_SCALAR(SDataBuffer);
    ALLOCATE_SCALAR(SVectorBuffer)
    ALLOCATE_SCALAR(SVectorBufferEnd);

    // Labels
    auto vectorLoop = createLabel();

    c.li(*SVectorBuffer, vectorPtr);
    c.li(*SVectorBufferEnd, vectorPtr + (numVectors * 64));

    // SDataBuffer = scalarPtr
    c.li(*SDataBuffer, scalarPtr);

    // Loop over vectors
    c.L(vectorLoop);
    {
        // Register allocation
        ALLOCATE_VECTOR(VData);
        ALLOCATE_SCALAR(SVal);

        // Load vector
        c.vloadv(*VData, *SVectorBuffer, 0);

        // **STALL**
        c.nop();
        
        // Unroll lane loop
        for(int l = 0; l < 32; l++) {
            // Extract lane into scalar registers
            c.vextract(*SVal, *VData, l);

            // Store halfword
            c.sh(*SVal, *SDataBuffer, l * 2);
        }

        // SVectorBuffer += 64
        c.addi(*SVectorBuffer, *SVectorBuffer, 64);

        // SDataBuffer += 64
        c.addi(*SDataBuffer, *SDataBuffer, 64);
      
        // If SVectorBuffer != SVectorBufferEnd, goto vector loop
        c.bne(*SVectorBuffer, *SVectorBufferEnd, vectorLoop);
    }
}
//----------------------------------------------------------------------------
void generateLaneLocalScalarMemcpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                   ScalarRegisterAllocator &scalarRegisterAllocator,
                                   uint32_t laneLocalPtr, uint32_t scalarPtr, uint32_t numVectors)
{
    // Register allocation
    ALLOCATE_SCALAR(SDataBuffer);
    ALLOCATE_SCALAR(SDataBufferEnd);
    ALLOCATE_VECTOR(VAddress)
    ALLOCATE_VECTOR(VTwo);

    // Labels
    auto vectorLoop = createLabel();

    c.vlui(*VAddress, laneLocalPtr);
    c.vlui(*VTwo, 2);

    // SDataBuffer = scalarPtr
    c.li(*SDataBuffer, scalarPtr);
    c.li(*SDataBufferEnd, scalarPtr + (64 * numVectors));

    // Loop over vectors
    c.L(vectorLoop);
    {
        // Register allocation
        ALLOCATE_VECTOR(VData);
        ALLOCATE_SCALAR(SVal);

        // Load from lane-local memory into vector and increment address
        c.vloadl(*VData, *VAddress, 0);
        c.vadd(*VAddress, *VAddress, *VTwo);
        
        // Unroll lane loop
        for(int l = 0; l < 32; l++) {
            // Extract lane into scalar registers
            c.vextract(*SVal, *VData, l);

            // Store halfword
            c.sh(*SVal, *SDataBuffer, l * 2);
        }

        // SDataBuffer += 64
        c.addi(*SDataBuffer, *SDataBuffer, 64);
      
        // If SDataBuffer != SDataBufferEnd, goto vector loop
        c.bne(*SDataBuffer, *SDataBufferEnd, vectorLoop);
    }
}
//----------------------------------------------------------------------------
void generateScalarLaneLocalBroadcast(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                      ScalarRegisterAllocator &scalarRegisterAllocator,
                                      uint32_t scalarPtr, uint32_t laneLocalPtr, uint32_t numHalfWords)
{
    assert(numHalfWords > 0);

    // Register allocation
    ALLOCATE_SCALAR(SDataBuffer);
    ALLOCATE_SCALAR(SDataBufferEnd);
    ALLOCATE_VECTOR(VAddress)
    ALLOCATE_VECTOR(VTwo);

    // Labels
    auto halfWordLoop = createLabel();

    // Load lane local memory address and increment
    c.vlui(*VAddress, laneLocalPtr);
    c.vlui(*VTwo, 2);

    // SDataBuffer = scalarPtr
    c.li(*SDataBuffer, scalarPtr);
    c.li(*SDataBufferEnd, scalarPtr + (2 * numHalfWords));

    // Loop over vectors
    c.L(halfWordLoop);
    {
        // Register allocation
        ALLOCATE_VECTOR(VVector);
        ALLOCATE_SCALAR(SVal);

        // Load halfword
        c.lh(*SVal, *SDataBuffer);

        // Increment pointer
        c.addi(*SDataBuffer, *SDataBuffer, 2);
    
        // Fill vector register
        c.vfill(*VVector, *SVal);

        // Write to all lane local memories and increment address
        c.vstorel(*VVector, *VAddress);
        c.vadd(*VAddress, *VAddress, *VTwo);

        // Loop
        c.bne(*SDataBuffer, *SDataBufferEnd, halfWordLoop);
    }
}
//----------------------------------------------------------------------------
void generatePerformanceCountWrite(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                                   Common::CSR lowCSR, Common::CSR highCSR, uint32_t scalarPtr)
{
    ALLOCATE_SCALAR(SAddress);
    ALLOCATE_SCALAR(SValue);
    
    c.li(*SAddress, scalarPtr);
    c.csrr(*SValue, lowCSR);
    c.sw(*SValue, *SAddress);
    c.csrr(*SValue, highCSR);
    c.sw(*SValue, *SAddress, 4);
}
//----------------------------------------------------------------------------
void unrollLoopBody(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator, 
                    uint32_t numIterations, uint32_t maxUnroll, bool alwaysGenerateTail,
                    std::function<void(CodeGenerator&, uint32_t)> genBodyFn, 
                    std::function<void(CodeGenerator&, uint32_t)> genTailFn)
{
    // Determine number of unrolled iterations and remainder
    const auto numUnrolls = std::div(static_cast<int64_t>(numIterations), maxUnroll);

    // If there are are complete unrolls
    ALLOCATE_SCALAR(SNumIterations);
    if(numUnrolls.quot != 0) {
        // Load number of unrolled iterations
        c.li(*SNumIterations, numUnrolls.quot);

        auto unrolledLoop = c.L();
        {
            // Unroll loop
            for(uint32_t r = 0; r < maxUnroll; r++) {
                genBodyFn(c, r);
            }

            // If more than 1 unroll is required or there are more iterations, generate tail
            if(numUnrolls.quot > 1 || numUnrolls.rem != 0 || alwaysGenerateTail) {
                genTailFn(c, maxUnroll);
            }

            // If more than 1 unroll is required
            if(numUnrolls.quot > 1) {
                // Decrement increment count and loop if still > 0
                c.addi(*SNumIterations, *SNumIterations, -1);
                c.bgt(*SNumIterations, FeNN::Common::Reg::X0, unrolledLoop);
            }
        }
    }

    // If there is a remainder
    if(numUnrolls.rem != 0) {
        // Load number of iterations
        c.li(*SNumIterations, numUnrolls.rem);

        auto loop = c.L();
        {
            // Generate body
            genBodyFn(c, 0);

            // Generate tail if required
            if (numUnrolls.rem > 1 || alwaysGenerateTail) {
                genTailFn(c, 1);
            }

            // If more than 1 iteration is required
            if(numUnrolls.quot > 1) {
                // Decrement increment count and loop if still > 0
                c.addi(*SNumIterations, *SNumIterations, -1);
                c.bgt(*SNumIterations, FeNN::Common::Reg::X0, loop);
            }
        }
    }
}
//----------------------------------------------------------------------------
void unrollLoopBody(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator, 
                    FeNN::Common::Reg countReg, uint32_t maxUnroll, uint32_t iterationSize, 
                    std::function<void(CodeGenerator&, uint32_t)> genBodyFn, 
                    std::function<void(CodeGenerator&, uint32_t)> genTailFn)
{  
    ALLOCATE_SCALAR(SLoopCountEnd);

    // Unrolled loop
    {
        // Load end count for unrolled loop
        c.li(*SLoopCountEnd, maxUnroll * iterationSize);

        auto unrolledLoopEnd = createLabel();
        auto unrolledLoopStart = c.L();
        {
            // If count is less than the size of a single unrolled iteration, leave loop
            c.bltu(countReg, *SLoopCountEnd, unrolledLoopEnd);

            // Generate unrolled body
            for(uint32_t r = 0; r < maxUnroll; r++) {
                genBodyFn(c, r);
            }

            // Generate tail
            genTailFn(c, maxUnroll);

            // Subtract unrolled iteration size from count
            c.sub(countReg, countReg, *SLoopCountEnd);
            c.j_(unrolledLoopStart);
        }
        c.L(unrolledLoopEnd);
    }

    // Tail loop
    {
        // Load end count for tail loop
        c.li(*SLoopCountEnd, iterationSize);

        auto tailLoopEnd = createLabel();
        auto tailLoopStart = c.L();
        {
            // If count is less than the size of a single iteration, leave loop
            // **NOTE** this seems a bit weird but enables this function to be used for vector loops
            c.bltu(countReg, *SLoopCountEnd, tailLoopEnd);

            // Generate body
            genBodyFn(c, 0);

            // Generate tail
            genTailFn(c, 1);

            // Subtract iteration size from count
            c.sub(countReg, countReg, *SLoopCountEnd);
            c.j_(tailLoopStart);
        }
        c.L(tailLoopEnd);
    }
}
//----------------------------------------------------------------------------
void unrollVectorLoopBody(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator, 
                          uint32_t numElements, uint32_t maxUnroll,
                          std::function<void(CodeGenerator&, uint32_t, ScalarRegisterAllocator::RegisterPtr)> genBodyFn, 
                          std::function<void(CodeGenerator&, uint32_t)> genTailFn)
{
    // Determine number of vectorised iterations and remainder
    const auto numVectors = std::div(numElements, 32);

    // If there are any complete vector iterations, unroll them
    // **NOTE** always generate a tail if there will be a partial vector to follow
    if(numVectors.quot != 0) {
        unrollLoopBody(c, scalarRegisterAllocator, numVectors.quot, 
                       maxUnroll, (numVectors.rem != 0),
                       [genBodyFn](CodeGenerator &c, uint32_t r)
                       { 
                           genBodyFn(c, r, nullptr); 
                       }, 
                       genTailFn);
    }

    // If there is a remainder
    if(numVectors.rem != 0) {
        ALLOCATE_SCALAR(SMask);

        // Calculate mask for final iteration
        c.li(*SMask, (1 << numVectors.rem) - 1);

        // Generate body with mask
        genBodyFn(c, 0, SMask);
    }
}
//----------------------------------------------------------------------------
void unrollVectorLoopBody(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator, 
                          FeNN::Common::Reg numElementsReg, uint32_t maxUnroll, bool noTail,
                          std::function<void(CodeGenerator&, uint32_t, ScalarRegisterAllocator::RegisterPtr)> genBodyFn, 
                          std::function<void(CodeGenerator&, uint32_t)> genTailFn)
{
    // Generate unrolled and whole vector loop
    unrollLoopBody(c, scalarRegisterAllocator, numElementsReg, maxUnroll, 32,
                   [genBodyFn](CodeGenerator &c, uint32_t r)
                   { 
                       genBodyFn(c, r, nullptr); 
                   }, 
                   genTailFn);

    // If a tail is required
    if (!noTail) {
        ALLOCATE_SCALAR(SMask);

        {
            ALLOCATE_SCALAR(SOne);

            // Calculate mask for final iteration
            c.li(*SOne, 1);
            c.sll(*SMask, *SOne, numElementsReg);
            c.sub(*SMask, *SMask, *SOne);
        }

        // Generate body with mask
        genBodyFn(c, 0, SMask);
    }
}
//----------------------------------------------------------------------------
std::vector<uint32_t> generateStandardKernel(bool simulate, uint32_t readyFlagPtr, 
                                             std::function<void(CodeGenerator&, VectorRegisterAllocator&, ScalarRegisterAllocator&)> genBodyFn)
{
    using namespace Common;

    CodeGenerator c;
    VectorRegisterAllocator vectorRegisterAllocator;
    ScalarRegisterAllocator scalarRegisterAllocator;
    
    // If we're simulating, generate body and add ecall instruction
    if(simulate) {
        genBodyFn(c, vectorRegisterAllocator, scalarRegisterAllocator);
        c.ecall();
    }
    // Otherwise
    else {
        // Register allocation
        ALLOCATE_SCALAR(SReadyFlagBuffer);

        // Labels
        auto spinLoop = createLabel();

        // Load ready flag
        c.li(*SReadyFlagBuffer, readyFlagPtr);
        
        // Clear ready flag
        c.sw(Reg::X0, *SReadyFlagBuffer);

        // Generate body
        genBodyFn(c, vectorRegisterAllocator, scalarRegisterAllocator);

        // Set ready flag
        {
            ALLOCATE_SCALAR(STmp);
            c.li(*STmp, 1);
            c.sw(*STmp, *SReadyFlagBuffer);
        }

        // Infinite loop
        c.L(spinLoop);
        {
            // **HACK** 
            //c.j_(spinLoop);
            c.beq(Reg::X0, Reg::X0, spinLoop);
        }
    }

    LOGI << "Max vector registers used: " << vectorRegisterAllocator.getMaxUsedRegisters();
    LOGI << "Max scalar registers used: " << scalarRegisterAllocator.getMaxUsedRegisters();

    return c.getCode();
}
//----------------------------------------------------------------------------
void generateSubtractUint64(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                            Common::Reg destLow, Common::Reg destHigh, Common::Reg subtractLow, Common::Reg subtractHigh)
{
    ALLOCATE_SCALAR(STemp);

    c.mv(*STemp, destLow);
    c.sub(destLow, destLow, subtractLow);
    c.sltu(*STemp, *STemp, destLow);
    c.sub(destHigh, destHigh, subtractHigh);
    c.sub(destHigh, destHigh, *STemp);
}
//----------------------------------------------------------------------------
void generateAddUint64(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator,
                       Common::Reg destLow, Common::Reg destHigh, Common::Reg addLow, Common::Reg addHigh)
{
    ALLOCATE_SCALAR(STemp);

    c.mv(*STemp, destLow);
    c.add(destLow, destLow, addLow);
    c.sltu(*STemp, destLow, *STemp);
    c.add(destHigh, destHigh, addHigh);
    c.add(destHigh, *STemp, destHigh);
}
//----------------------------------------------------------------------------
void generateDMAStartWrite(CodeGenerator &c, Common::Reg destination, Common::Reg source, Common::Reg size)
{
    using namespace Common;

    // Write source and destination addresses to CSRs
    c.csrw(CSR::MM2S_SRC_ADDR, source);
    c.csrw(CSR::MM2S_DST_ADDR, destination);

    // Write count to CSR
    c.csrw(CSR::MM2S_COUNT, size);

    // Run
    // **THINK** fancier CSR instruction could be used here
    c.csrwi(CSR::MM2S_CONTROL, 1);
}
//----------------------------------------------------------------------------
void generateDMAStartRead(CodeGenerator &c, Common::Reg destination, Common::Reg source, Common::Reg size)
{
    using namespace Common;

    // Write source and destination addresses to CSRs
    c.csrw(CSR::S2MM_SRC_ADDR, source);
    c.csrw(CSR::S2MM_DST_ADDR, destination);

    // Write count to CSR
    c.csrw(CSR::S2MM_COUNT, size);

    // Run
    // **THINK** fancier CSR instruction could be used here
    c.csrwi(CSR::S2MM_CONTROL, 1);
}
//----------------------------------------------------------------------------
ScalarRegisterAllocator::RegisterPtr generateDMAWaitForWriteComplete(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator)
{
    using namespace Common;

    ALLOCATE_SCALAR(SStatus);
    ALLOCATE_SCALAR(SIdle);

    auto loop = createLabel();
    c.L(loop);
    {
        // Read status register
        c.csrr(*SStatus, CSR::MM2S_STATUS);

        // Extract idle bit
        c.andi(*SIdle, *SStatus, 1 << 1);// static_cast<uint32_t>(StatusBits::STATE_IDLE))

        // Loop while not idle
        c.beq(*SIdle, Reg::X0, loop);
    }

    // Return status register
    return SStatus;
}
//----------------------------------------------------------------------------
ScalarRegisterAllocator::RegisterPtr generateDMAWaitForReadComplete(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator)
{
    using namespace Common;

    ALLOCATE_SCALAR(SStatus);
    ALLOCATE_SCALAR(SIdle);

    auto loop = createLabel();
    c.L(loop);
    {
        // Read status register
        c.csrr(*SStatus, CSR::S2MM_STATUS);

        // Extract idle bit
        c.andi(*SIdle, *SStatus, 1 << 1);// static_cast<uint32_t>(StatusBits::STATE_IDLE))

        // Loop while not idle
        c.beq(*SIdle, Reg::X0, loop);
    }

    // Return status register
    return SStatus;
}
//----------------------------------------------------------------------------
void generateRouterBarrier(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator, uint32_t numMasters)
{
    using namespace Common;

    // Send barrier event from this core
    c.csrwi(CSR::MASTER_SEND_BARRIER, 1);

    // Load target number of masters to wait for
    ALLOCATE_SCALAR(SNumMasters);
    ALLOCATE_SCALAR(SBarrierCount);
    c.li(*SNumMasters, numMasters);

    auto loop = createLabel();
    c.L(loop);
    {
        // Read barrier count register
        c.csrr(*SBarrierCount, CSR::SLAVE_BARRIER_COUNT);

        // Loop while target count isn't reached
        c.bne(*SBarrierCount, *SNumMasters, loop);
    }

    // Clear barrier count
    c.csrwi(CSR::SLAVE_BARRIER_COUNT, 0);
}
}
