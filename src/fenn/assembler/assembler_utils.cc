#include "fenn/assembler/assembler_utils.h"

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
                    uint32_t numIterations, uint32_t maxUnroll, uint32_t iterationBytes,
                    Common::Reg testBufferReg, bool alwaysGenerateTail,
                    std::function<void(CodeGenerator&, uint32_t, bool)> genBodyFn, 
                    std::function<void(CodeGenerator&, uint32_t)> genTailFn)
{
    // Evenness of iterations can only be determined with even numbers of unrolls
    assert((maxUnroll % 2) == 0);

    // Determine number of unrolled iterations and remainder
    const auto numUnrolls = std::div(static_cast<int64_t>(numIterations), maxUnroll);
    
    // If there are are complete unrolls
    if(numUnrolls.quot != 0) {
        // Calculate end of unrolled section of buffer
        ALLOCATE_SCALAR(STestBufferEndReg);
        const size_t stride = iterationBytes * numUnrolls.quot * maxUnroll;
        if(Common::inSBit(stride, 12)) {
            c.addi(*STestBufferEndReg, testBufferReg, stride);
        }
        else {
            c.li(*STestBufferEndReg, stride);
            c.add(*STestBufferEndReg, *STestBufferEndReg, testBufferReg);
        }

        auto loop = createLabel();
        c.L(loop);
        {
            // Unroll loop
            for(uint32_t r = 0; r < maxUnroll; r++) {
                genBodyFn(c, r, (r % 2) == 0);
            }

            // If more than 1 unroll is required or there are more iterations, generate tail
            if(numUnrolls.quot > 1 || numUnrolls.rem != 0 || alwaysGenerateTail) {
                genTailFn(c, maxUnroll);
            }
            
            // If more than 1 unroll is required, generate loop
            if(numUnrolls.quot > 1) {
                c.bne(testBufferReg, *STestBufferEndReg, loop);
            }
        }
    }

    // If there is a remainder
    if(numUnrolls.rem != 0) {
        // Unroll tail
        for(uint32_t r = 0; r < numUnrolls.rem; r++) {
            genBodyFn(c, r, (r % 2) == 0);
        }

        // If we should always generate a tail, do so
        if(alwaysGenerateTail) {
            genTailFn(c, numUnrolls.rem);
        }
        
    }
}
//----------------------------------------------------------------------------
void unrollVectorLoopBody(CodeGenerator &c, ScalarRegisterAllocator &scalarRegisterAllocator, 
                          uint32_t numIterations, uint32_t maxUnroll, Common::Reg testBufferReg,
                          std::function<void(CodeGenerator&, uint32_t, bool, ScalarRegisterAllocator::RegisterPtr)> genBodyFn, 
                          std::function<void(CodeGenerator&, uint32_t)> genTailFn)
{
    // Determine number of vectorised iterations and remainder
    const auto numVectors = std::div(numIterations, 32);
    
    // If there are any complete vector iterations, unroll them
    // **NOTE** always generate a tail if there will be a partial vector to follow
    if(numVectors.quot != 0) {
        unrollLoopBody(c, scalarRegisterAllocator, numVectors.quot, maxUnroll, 
                       64, testBufferReg, (numVectors.rem != 0),
                       [genBodyFn](CodeGenerator &c, uint32_t r, bool even)
                       { 
                           genBodyFn(c, r, even, nullptr); 
                       }, 
                       genTailFn);
    }

    // If there is a remainder
    if(numVectors.rem != 0) {
        ALLOCATE_SCALAR(SMask);

        // Calculate mask for final iteration
        c.li(*SMask, (1 << numVectors.rem) - 1);
        
        // Generate body with mask
        genBodyFn(c, 0, (numVectors.quot % 2) == 0, SMask);
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
