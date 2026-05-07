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
                    uint32_t numPre, uint32_t numPost, uint32_t stride, bool debug)
{
    // Register allocation
    ALLOCATE_SCALAR(SWordNStart);
    ALLOCATE_SCALAR(SConst1);
    ALLOCATE_SCALAR(SSpikeWord);
    ALLOCATE_SCALAR(SISynBuffer);
    ALLOCATE_SCALAR(SStride);

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
        
    c.li(*SStride, stride);

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

        // CNumLZ = clz(SSpikeWord);
        c.clz(*SNumLZ, *SSpikeWord);

        // If SSpikeWord == 1  i.e. CNumLZ == 31, goto zeroSpikeWord
        c.beq(*SSpikeWord, *SConst1, zeroSpikeWord);

        {
            // STmp = CNumLZ + 1
            ALLOCATE_SCALAR(STmp);
            c.addi(*STmp, *SNumLZ, 1);

            // SSpikeWord <<= CNumLZPlusOne
            c.sll(*SSpikeWord, *SSpikeWord, *STmp);
        }

        // SN -= SNumLZ
        c.L(bitLoopBody);
        c.sub(*SN, *SN, *SNumLZ);

        // SWeightBuffer = weightInHidStart + (numPostVecs * 64 * SN);
        ALLOCATE_SCALAR(SWeightBuffer);
        c.li(*SWeightBuffer, weightPtr);
        {
            ALLOCATE_SCALAR(STemp);
            c.mul(*STemp, *SN, *SStride);
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
                           numInput, numHidden, 64, false);

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
