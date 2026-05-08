// Standard C++ includes
#include <bitset>
#include <fstream>
#include <iterator>
#include <limits>
#include <memory>

// Standard C includes
#include <cassert>
#include <cmath>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V utils include
#include "common/app_utils.h"
#include "common/device.h"
#include "common/dma_buffer.h"
#include "common/dma_controller.h"
#include "common/utils.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"


void genStaticPulse(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                    ScalarRegisterAllocator &scalarRegisterAllocator, uint32_t weightPtr,
                    std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> preSpikePtr, uint32_t postISynPtr,
                    uint32_t numPre, uint32_t matRows, uint32_t matCols, uint32_t numCols, bool debug)
{
    // Register allocation
    ALLOCATE_SCALAR(SSpikeBufferEnd);
    ALLOCATE_SCALAR(SWordNStart);
    ALLOCATE_SCALAR(SConst1);
    ALLOCATE_SCALAR(SSpikeWord);
    ALLOCATE_SCALAR(SISynBuffer);
    ALLOCATE_SCALAR(SNumCols);
    ALLOCATE_SCALAR(SResAlignMask);
    ALLOCATE_SCALAR(SBaseTileSelectMask);
    ALLOCATE_SCALAR(SWeightBuffer);
    
    // Labels
    Label wordLoop;
    Label bitLoopStart;
    Label bitLoopBody;
    Label bitLoopEnd;
    Label zeroSpikeWord;
    Label wordEnd;

    // If literal is provided for start of presynapric spike buffer, allocate register and load immediate into it
    ScalarRegisterAllocator::RegisterPtr SSpikeBuffer;
    if (std::holds_alternative<uint32_t>(preSpikePtr)) {
        SSpikeBuffer = scalarRegisterAllocator.getRegister("SSpikeBuffer = X");
        c.li(*SSpikeBuffer, std::get<uint32_t>(preSpikePtr));
    }
    // Otherwise, use pointer register directly
    else {
        SSpikeBuffer = std::get<ScalarRegisterAllocator::RegisterPtr>(preSpikePtr);
    }

    // Get address of end of presynaptic spike buffer
    c.li(*SSpikeBufferEnd, (ceilDivide(numPre, 32) * 4));
    c.add(*SSpikeBufferEnd, *SSpikeBufferEnd, *SSpikeBuffer);

    // SISynBuffer = hiddenIsyn;
    c.li(*SISynBuffer, postISynPtr);

    // Load some useful constants
    c.li(*SConst1, 1);

    // SWordNStart = 31
    c.li(*SWordNStart, 31);

    // **NOTE** not necessary on FeNN HW
    c.li(*SResAlignMask, 0xFFFFFFE0);

    // Create base mask to select a single row from tile
    c.li(*SBaseTileSelectMask, (1 << matCols) - 1);

    c.li(*SWeightBuffer, weightPtr);

    c.li(*SNumCols, numCols);

    // Outer word loop
    c.L(wordLoop);
    {
        // Register allocation
        ALLOCATE_SCALAR(SN);

        // SSpikeWord = *SSpikeBuffer++
        c.lw(*SSpikeWord, *SSpikeBuffer);
        c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);

        // If SSpikeWord == 0, goto bitloop end
        c.beq(*SSpikeWord, Reg::X0, bitLoopEnd);

        // SN = SWordNStart
        c.mv(*SN, *SWordNStart);

        // Inner bit loop
        c.L(bitLoopStart);
        {
            // Register allocation
            ALLOCATE_SCALAR(SNumLZ);
            ALLOCATE_SCALAR(SNumLZPlusOne);

            // CNumLZ = clz(SSpikeWord);
            c.clz(*SNumLZ, *SSpikeWord);

            // If SSpikeWord == 1  i.e. CNumLZ == 31, goto zeroSpikeWord
            c.beq(*SSpikeWord, *SConst1, zeroSpikeWord);

            // CNumLZPlusOne = CNumLZ + 1
            c.addi(*SNumLZPlusOne, *SNumLZ, 1);

            // SSpikeWord <<= CNumLZPlusOne
            c.sll(*SSpikeWord, *SSpikeWord, *SNumLZPlusOne);

            // SN -= SNumLZ
            c.L(bitLoopBody);
            c.sub(*SN, *SN, *SNumLZ);

            {
                ALLOCATE_SCALAR(SWeightRowBuffer);
                ALLOCATE_SCALAR(SISynRowOffset);
                ALLOCATE_SCALAR(SISynRowBuffer);
                ALLOCATE_SCALAR(SISynRowBufferBytes);
                ALLOCATE_SCALAR(STileSelectMask);
                ALLOCATE_VECTOR(VISyn);
                ALLOCATE_VECTOR(VWeightTile);

                // Calculate shift to extract row from SN
                assert(isPOT(matRows));
                const int matRowShift = ctz(matRows);

                // Extract rows and columns
                c.srli(*SISynRowOffset, *SN, matRowShift);
                c.andi(*SWeightRowBuffer, *SN, matRows - 1);

                // Multiply column by 32*2 and add to weight address
                c.slli(*SWeightRowBuffer, *SWeightRowBuffer, 6);
                c.add(*SWeightRowBuffer, *SWeightRowBuffer, *SWeightBuffer);

                // Get postsynaptic index (half-words)
                c.mul(*SISynRowOffset, *SISynRowOffset, *SNumCols);

                // Align (half-words)
                // **NOTE** this is not necessary on FeNN HW
                c.and_(*SISynRowBuffer, *SISynRowOffset, *SResAlignMask);

                // **YUCK** convert to bytes
                c.slli(*SISynRowBufferBytes, *SISynRowBuffer, 1);

                // Load ISyn
                c.add(*SISynRowBufferBytes, *SISynBuffer, *SISynRowBufferBytes);
                c.vloadv(*VISyn, *SISynRowBufferBytes);
                
                // Load weight
                c.vloadv(*VWeightTile, *SWeightRowBuffer);

                // Calculate remaining  (half-words)
                c.sub(*SISynRowOffset, *SISynRowBuffer, *SISynRowOffset);

                // Add weight
                c.vadd_s(*VWeightTile, *VWeightTile, *VISyn);

                // Build tile select mask (half-words)
                c.sll(*STileSelectMask, *SBaseTileSelectMask, *SISynRowOffset);

                // VISyn = STileSelectMask ? VWeightTile : VISyn
                c.vsel(*VISyn, *STileSelectMask, *VWeightTile);

                // SN --
                // **NOTE** just to fill stall slot
                c.addi(*SN, *SN, -1);

                // Store updated ISyn
                c.vstore(*VISyn, *SISynRowBufferBytes);
            }
            
            // Load weight and Isyn
            if (debug) {
                c.ebreak();
            }   

            // If SSpikeWord != 0, goto bitLoopStart
            c.bne(*SSpikeWord, Reg::X0, bitLoopStart);
        }

        // SWordNStart += 32
        c.L(bitLoopEnd);
        c.addi(*SWordNStart, *SWordNStart, 32);

        // If SSpikeBuffer != SSpikeBufferEnd, goto wordloop
        c.bne(*SSpikeBuffer, *SSpikeBufferEnd, wordLoop);

        // Goto wordEnd
        c.j_(wordEnd);
    }

    // Zero spike word
    {
        c.L(zeroSpikeWord);
        c.li(*SSpikeWord, 0);
        c.j_(bitLoopBody);
    }

    c.L(wordEnd);
}

int main()
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Allocate memory
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Constants
    constexpr bool simulate = true;
    constexpr uint32_t numUIn = 250 * 1;
    constexpr uint32_t numMIn = 250 * 4;
    constexpr uint32_t numUOut = 250 * 4;
    constexpr uint32_t numMOut = 250 * 4;
    constexpr uint32_t numUSpikeWords = ceilDivide(numUIn, 32);
    constexpr uint32_t numMSpikeWords = ceilDivide(numMIn, 32);
    
    // Allocate vector arrays
    // **NOTE** adjacent so can be block copied from scalar
    const uint32_t aWeightPtr = AppUtils::loadVectors("a.bin", vectorInitData);
    const uint32_t bWeightPtr = AppUtils::loadVectors("b.bin", vectorInitData);
    
    const uint32_t uOutPtr = AppUtils::allocateVectorAndZero(numUOut, vectorInitData);
    const uint32_t mOutPtr = AppUtils::allocateVectorAndZero(numMOut, vectorInitData);
    
    // Allocate scalar arrays
    const uint32_t uSpikePtr = AppUtils::loadScalars("spk_u.bin", scalarInitData);
    const uint32_t mSpikePtr = AppUtils::loadScalars("spk_m.bin", scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    
    // Generate sim code
    const auto simCode = AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // spk_m * A
            //genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator,
            //               aWeightPtr, mSpikePtr, mOutPtr,
            //               numMIn, 4, 4, 4, false);
            
            // spk_u * B
            genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator,
                           bWeightPtr, uSpikePtr, uOutPtr,
                           numUIn, 1, 4, 4, false);
            
        });

    // Dump to coe file
    AppUtils::dumpCOE("l2mu.coe", simCode);
    LOGI << simCode.size() << " simulation instructions";
    LOGI << scalarInitData.size() << " bytes of scalar memory required";
    LOGI << vectorInitData.size() * 2 << " bytes of vector memory required (" << ceilDivide(vectorInitData.size() / 32, 4096) << " URAM cascade)";

    
    if(simulate) {
        RISCV riscV;
        riscV.setInstructions(simCode);
        riscV.getScalarDataMemory().setData(scalarInitData);
        
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);
        riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().setData(vectorInitData);
        
        if(!riscV.run()) {
            return 1;
        }

        auto *vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData();
        const int16_t *uOut = vectorData + (uOutPtr / 2);
        const int16_t *mOut = vectorData + (mOutPtr / 2);
        std::cout << "M" << std::endl;
        for(uint32_t i = 0; i < numMOut; i++) {
            std::cout << *mOut++ << ", ";
        }
        std::cout << std::endl;
        
        std::cout << "U" << std::endl;
        for (uint32_t i = 0; i < numUOut; i++) {
            std::cout << *uOut++ << ", ";
        }
        std::cout << std::endl;

    }
    else {
        /*LOGI << "Creating device";
        Device device;
        LOGI << "Resetting";
        // Put core into reset state
        device.setEnabled(false);
        
        LOGI << "Copying instructions (" << simCode.size() * sizeof(uint32_t) << " bytes)";
        device.uploadCode(simCode);
        
        LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
        device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());
        
        LOGI << "Enabling";
        // Put core into running state
        device.setEnabled(true);
        LOGI << "Running";
        
        // Wait until ready flag
        device.waitOnNonZero(readyFlagPtr);
        LOGI << "Done";
        device.setEnabled(false);
        
        int16_t hiddenIsyn[32];
        device.memcpyDataFromDevice(reinterpret_cast<uint8_t*>(&hiddenIsyn[0]), hiddenIsynScalarPtr, numHidden * 2);
        check(hiddenIsyn, numInput, numHidden);*/
        
    }
    return 0;
}
