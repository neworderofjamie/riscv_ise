#include "assembler/assembler_utils.h"

//----------------------------------------------------------------------------
// AssemblerUtils
//----------------------------------------------------------------------------
namespace AssemblerUtils
{
void generateScalarVectorMemcpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                ScalarRegisterAllocator &scalarRegisterAllocator,
                                std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> scalarPtr, 
                                std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> vectorPtr, 
                                std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> numVectors)
{
    // Register allocation
    ALLOCATE_SCALAR(SVectorBufferEnd);

    // Labels
    Label vectorLoop;

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
        c.add(*SVectorBufferEnd, *SVectorBuffer, *std::get<ScalarRegisterAllocator::RegisterPtr>(numVectors));
    }

    // Loop over vectors
    c.L(vectorLoop);
    {
        // Register allocation
        ALLOCATE_VECTOR(VData);

        // Unroll lane loop
        for(int l = 0; l < 32; l++) {
            // Register allocation
            ALLOCATE_VECTOR(VLane);
            ALLOCATE_SCALAR(SMask);
            ALLOCATE_SCALAR(SVal);

            // Load halfword
            c.lh(*SVal, *SDataBuffer, l * 2);

            // SMask = 1 << SLane
            c.li(*SMask, 1 << l);

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
    Label vectorLoop;

    c.li(*SVectorBuffer, vectorPtr);
    c.li(*SVectorBufferEnd, vectorPtr + (numVectors * 64));

    // SDataBuffer = scalarPtr
    c.li(*SDataBuffer, scalarPtr);

    // Loop over vectors
    c.L(vectorLoop);
    {
        // Register allocation
        ALLOCATE_VECTOR(VData);

        // Load vector
        c.vloadv(*VData, *SVectorBuffer, 0);
        
        // Unroll lane loop
        for(int l = 0; l < 32; l++) {
            // Register allocation
            ALLOCATE_SCALAR(SVal);
            
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
void unrollLoopBody(CodeGenerator &c, uint32_t numIterations, uint32_t maxUnroll, 
                    Reg testBufferReg, Reg testBufferEndReg, 
                    std::function<void(CodeGenerator&, uint32_t)> genBodyFn, 
                    std::function<void(CodeGenerator&, uint32_t)> genTailFn)
{
    // **TODO** tail loop after unrolling
    assert((numIterations % maxUnroll) == 0);
    const uint32_t numUnrolls = std::min(numIterations, maxUnroll);
    const uint32_t numUnrolledIterations = numIterations / numUnrolls;

    // Input postsynaptic neuron loop
    Label loop;
    c.L(loop);
    {
        // Unroll loop
        for(uint32_t r = 0; r < numUnrolls; r++) {
            genBodyFn(c, r);
        }

        genTailFn(c, numUnrolls);
                    
        // If we haven't reached end of Isyn buffer, loop
        if(numUnrolledIterations > 1) {
            c.bne(testBufferReg, testBufferEndReg, loop);
        }
    }
}
//----------------------------------------------------------------------------
void unrollVectorLoopBody(CodeGenerator &c, uint32_t numIterations, uint32_t maxUnroll, 
                          Reg testBufferReg, Reg testBufferEndReg, 
                          std::function<void(CodeGenerator&, uint32_t)> genBodyFn, 
                          std::function<void(CodeGenerator&, uint32_t)> genTailFn)
{
    // Only loop bodies for now
    assert((numIterations % 32) == 0);
    unrollLoopBody(c, numIterations / 32, maxUnroll, 
                   testBufferReg, testBufferEndReg,
                   genBodyFn, genTailFn);

}
}