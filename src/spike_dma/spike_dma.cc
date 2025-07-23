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
#include "common/utils.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"

void genStaticPulse(CodeGenerator &c, RegisterAllocator<VReg> &vectorRegisterAllocator,
                    RegisterAllocator<Reg> &scalarRegisterAllocator, uint32_t weightPtr, 
                    std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> preSpikePtr, uint32_t postISynPtr, 
                    uint32_t numPre, uint32_t numPost, uint32_t scaleShift, bool debug)
{
    // Register allocation
    ALLOCATE_SCALAR(SWordNStart);
    ALLOCATE_SCALAR(SConst1);
    ALLOCATE_SCALAR(SSpikeWord);
    ALLOCATE_SCALAR(SISynBuffer);

    assert(numPre == 32);
    assert(numPost == 32);

    // Labels
    Label bitLoopStart;
    Label bitLoopBody;
    Label zeroSpikeWord;
    Label wordEnd;

    // If literal is provided for start of presynapric spike buffer, allocate register and load immediate into it
    ScalarRegisterAllocator::RegisterPtr SSpikeBuffer;
    if(std::holds_alternative<uint32_t>(preSpikePtr)) {
        SSpikeBuffer = scalarRegisterAllocator.getRegister("SSpikeBuffer = X");
        c.li(*SSpikeBuffer, std::get<uint32_t>(preSpikePtr));
    }
    // Otherwise, use pointer register directly
    else {
        SSpikeBuffer = std::get<ScalarRegisterAllocator::RegisterPtr>(preSpikePtr);
    }
    
    // SISynBuffer = hiddenIsyn;
    c.li(*SISynBuffer, postISynPtr);
    
    // Load some useful constants
    c.li(*SConst1, 1);

    // SWordNStart = 31
    c.li(*SWordNStart, 31);
        
    // Register allocation
    ALLOCATE_SCALAR(SN);

    // SSpikeWord = *SSpikeBuffer++
    c.lw(*SSpikeWord, *SSpikeBuffer);

    // If SSpikeWord == 0, goto bitloop end
    c.beq(*SSpikeWord, Reg::X0, wordEnd);

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

        // SWeightBuffer = weightInHidStart + (numPostVecs * 64 * SN);
        // **TODO** multiply
        ALLOCATE_SCALAR(SWeightBuffer);
        c.li(*SWeightBuffer, weightPtr);
        {
            ALLOCATE_SCALAR(STemp);
            c.slli(*STemp, *SN, scaleShift);
            c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
        }

            
        // Load weight and Isyn
        if(debug) {
            c.ebreak();
        }

        {
            ALLOCATE_VECTOR(VWeight);
            ALLOCATE_VECTOR(VISyn);
         
            // Load next vector of weights and ISyns
            c.vloadv(*VWeight, *SWeightBuffer);
            c.vloadv(*VISyn, *SISynBuffer);

            // **STALL**
            c.nop();

            // Add weights to ISyn with mask
            c.vadd(*VISyn, *VISyn, *VWeight);
            
            // Write back ISyn
            c.vstore(*VISyn, *SISynBuffer);
        }


        // SN --
        c.addi(*SN, *SN, -1);
            
        // If SSpikeWord != 0, goto bitLoopStart
        c.bne(*SSpikeWord, Reg::X0, bitLoopStart);

        // Goto wordEnd
        //c.j_(wordEnd);
        c.beq(Reg::X0, Reg::X0, wordEnd);
    }

    // Zero spike word
    {
        c.L(zeroSpikeWord);
        c.li(*SSpikeWord, 0);
        //c.j_(bitLoopBody);
        c.beq(Reg::X0, Reg::X0, bitLoopBody);
    }
    
    c.L(wordEnd);
}

/*void genStaticPulse(CodeGenerator &c, RegisterAllocator<VReg> &vectorRegisterAllocator,
                    RegisterAllocator<Reg> &scalarRegisterAllocator, uint32_t weightPtr, 
                    std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> preSpikePtr, uint32_t postISynPtr, 
                    uint32_t numPre, uint32_t numPost, uint32_t scaleShift, bool debug)
{
    // Register allocation
    ALLOCATE_SCALAR(SSpikeBufferEnd);
    ALLOCATE_SCALAR(SWordNStart);
    ALLOCATE_SCALAR(SConst1);
    ALLOCATE_SCALAR(SSpikeWord);
    ALLOCATE_SCALAR(SISynBuffer);
    ALLOCATE_SCALAR(SISynBufferEnd);

    // Labels
    Label wordLoop;
    Label bitLoopStart;
    Label bitLoopBody;
    Label bitLoopEnd;
    Label zeroSpikeWord;
    Label wordEnd;

    // If literal is provided for start of presynapric spike buffer, allocate register and load immediate into it
    ScalarRegisterAllocator::RegisterPtr SSpikeBuffer;
    if(std::holds_alternative<uint32_t>(preSpikePtr)) {
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
    // **NOTE** is only the end of the vectorised region
    c.li(*SISynBuffer, postISynPtr);
    c.li(*SISynBufferEnd, postISynPtr + ((numPost / 32) * 64));

    // Load some useful constants
    c.li(*SConst1, 1);

    // SWordNStart = 31
    c.li(*SWordNStart, 31);
        
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

            // SWeightBuffer = weightInHidStart + (numPostVecs * 64 * SN);
            // **TODO** multiply
            ALLOCATE_SCALAR(SWeightBuffer);
            c.li(*SWeightBuffer, weightPtr);
            {
                ALLOCATE_SCALAR(STemp);
                c.slli(*STemp, *SN, scaleShift);
                c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
            }

            // Reset Isyn pointer
            c.li(*SISynBuffer, postISynPtr);
            
            // Load weight and Isyn
            if(debug) {
                c.ebreak();
            }

            // Loop over postsynaptic neurons
            if(numPost > 32) {
                ALLOCATE_VECTOR(VWeight);
                ALLOCATE_VECTOR(VISyn1);
                ALLOCATE_VECTOR(VISyn2);

                // Preload first ISyn to avoid stall
                c.vloadv(*VISyn1, *SISynBuffer, 0);

                AssemblerUtils::unrollVectorLoopBody(
                    c, numPost, 4, *SISynBuffer, *SISynBufferEnd,
                    [SWeightBuffer, SISynBuffer, VWeight, VISyn1, VISyn2]
                    (CodeGenerator &c, uint32_t r)
                    {
                        // Load vector of weights
                        c.vloadv(*VWeight, *SWeightBuffer, r * 64);

                        // Unless this is last unroll, load NEXT vector of ISyn to avoid stall
                        // **YUCK** in last iteration, while this may not be accessed, it may be out of bounds
                        const bool even = ((r % 2) == 0);
                        c.vloadv(even ? *VISyn2 : *VISyn1, *SISynBuffer, (r + 1) * 64);
                        
                        // Add weights to ISyn
                        auto VISyn = even ? VISyn1 : VISyn2;
                        c.vadd(*VISyn, *VISyn, *VWeight);

                        // Write back ISyn and increment SISynBuffer
                        c.vstore(*VISyn, *SISynBuffer, r * 64);
                    },
                    [SWeightBuffer, SISynBuffer]
                    (CodeGenerator &c, uint32_t numUnrolls)
                    {
                        // Increment pointers 
                        c.addi(*SISynBuffer, *SISynBuffer, 64 * numUnrolls);
                        c.addi(*SWeightBuffer, *SWeightBuffer, 64 * numUnrolls);
                    });
            }
            // Tail if there are non-POT number of postsynaptic neurons
            if((numPost % 32) != 0) {
                ALLOCATE_SCALAR(SMask);
                ALLOCATE_VECTOR(VWeight);
                ALLOCATE_VECTOR(VISyn);
                ALLOCATE_VECTOR(VISynNew);

                // Calculate mask for final iteration
                c.li(*SMask, (1 << (padSize(numPost, 32) - numPost)) - 1);

                // Load next vector of weights and ISyns
                c.vloadv(*VWeight, *SWeightBuffer);
                c.vloadv(*VISyn, *SISynBuffer);

                // **STALL**
                c.nop();

                // Add weights to ISyn with mask
                c.vadd(*VISynNew, *VISyn, *VWeight);
                c.vsel(*VISyn, *SMask, *VISynNew);

                // Write back ISyn
                c.vstore(*VISyn, *SISynBuffer);
            }


            // SN --
            c.addi(*SN, *SN, -1);
            
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
}*/

void check(const int16_t *hiddenIsyn, size_t numInput, size_t numHidden)
{
    int numCorrect = 0;
    for(size_t i = 0; i < numHidden; i++) {
        int16_t val = 0;
        for(size_t j = 0; j < numInput; j++) {
            if(0xDEADBEEF & (1u << j)) {
                val += ((32 * j) + i); 
            }
        }
        std::cout << hiddenIsyn[i] << "(" << val << "), ";
        if(val == hiddenIsyn[i]) {
            numCorrect++;
        }
    }
    std::cout << std::endl;
    std::cout << numCorrect << " correct" << std::endl;
}

std::vector<uint32_t> generateSimCode(bool simulate, uint32_t numInput, uint32_t numHidden, uint32_t numInputSpikeWords, uint32_t numHiddenSpikeWords, 
                                      uint32_t inputSpikePtr, uint32_t weightInHidPtr, uint32_t weightInHidScalarPtr, uint32_t hiddenIsynPtr, uint32_t hiddenIsynScalarPtr, 
                                      uint32_t readyFlagPtr)
{
    return AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Generate code to copy weights and in syn from scalar memory to vector memory
            AssemblerUtils::generateScalarVectorMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       weightInHidScalarPtr, weightInHidPtr, 
                                                       (numInput * numHiddenSpikeWords) + numHiddenSpikeWords);
            
            
            // 2^6 = 2 bytes * 32 hidden neurons
            genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator,
                           weightInHidPtr, inputSpikePtr, hiddenIsynPtr, 
                           numInput, numHidden, 6, false);

            // Copy Isyn to BRAM
            AssemblerUtils::generateVectorScalarMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       hiddenIsynPtr, hiddenIsynScalarPtr, numHiddenSpikeWords);
        });
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
    constexpr uint32_t numInput = 32;
    constexpr uint32_t numHidden = 32;
    constexpr uint32_t numInputSpikeWords = ceilDivide(numInput, 32);
    constexpr uint32_t numHiddenSpikeWords = ceilDivide(numHidden, 32);
    
    // Allocate vector arrays
    // **NOTE** adjacent so can be block copied from scalar
    const uint32_t weightInHidPtr = AppUtils::allocateVectorAndZero(numInput * numHiddenSpikeWords * 32, vectorInitData);
    const uint32_t hiddenIsynPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    
    // Allocate scalar arrays
    const uint32_t inputSpikePtr = AppUtils::allocateScalarAndZero(numInputSpikeWords * 4, scalarInitData);
    const uint32_t weightInHidScalarPtr = AppUtils::allocateScalarAndZero(numInput * numHiddenSpikeWords * 64, scalarInitData);
    const uint32_t hiddenIsynScalarPtr = AppUtils::allocateScalarAndZero(numHiddenSpikeWords * 64, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Generate weight data pattern
    {
        std::vector<int16_t> test(numInput * numHiddenSpikeWords * 32);
        for(size_t i = 0; i < numInput; i++) {
            for(size_t j = 0; j < numHidden; j++) {
                test[(i * numHiddenSpikeWords * 32) + j] = (32 * i) + j; 
            }
        }
        std::memcpy(scalarInitData.data() + weightInHidScalarPtr, test.data(), test.size() * 2);
    }

    // Generate spikes
    {
        std::vector<uint32_t> test(numInputSpikeWords);
        for(size_t i = 0; i < numInputSpikeWords; i++) {
            test[i] = 0xDEADBEEF;
        }
        std::memcpy(scalarInitData.data() + inputSpikePtr, test.data(), test.size() * 4);
    }
    
    std::vector<uint32_t> wordData(scalarInitData.size() / 4);
    std::memcpy(wordData.data(), scalarInitData.data(), scalarInitData.size());
    AppUtils::dumpCOE("spike_data.coe", wordData);
    
    // Generate sim code
    const auto simCode = generateSimCode(simulate, numInput, numHidden, numInputSpikeWords, numHiddenSpikeWords, 
                                         inputSpikePtr, weightInHidPtr, weightInHidScalarPtr, hiddenIsynPtr, hiddenIsynScalarPtr, readyFlagPtr);
    // Dump to coe file
    AppUtils::dumpCOE("spike.coe", simCode);
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

        auto *scalarData = riscV.getScalarDataMemory().getData();
        const int16_t *hiddenIsyn = reinterpret_cast<const int16_t*>(scalarData + hiddenIsynScalarPtr);
        check(hiddenIsyn, numInput, numHidden);

    }
    else {
        LOGI << "Creating device";
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
        check(hiddenIsyn, numInput, numHidden);
        
    }
    return 0;
}
