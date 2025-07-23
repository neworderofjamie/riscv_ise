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

// RISC-V common include
#include "common/CLI11.hpp"
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
#include "ise/dma_controller_sim.h"
#include "ise/riscv.h"
#include "ise/vector_processor.h"

void genStaticPulse(CodeGenerator &c, RegisterAllocator<VReg> &vectorRegisterAllocator,
                    ScalarRegisterAllocator &scalarRegisterAllocator, uint32_t weightPtr, 
                    std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> preSpikePtr, 
                    uint32_t postISynPtr, uint32_t rowBufferAPtr, uint32_t rowBufferBPtr,
                    uint32_t numPre, uint32_t numPost, uint32_t stride, bool debug)
{
    // Register allocation
    ALLOCATE_SCALAR(SWordNStart);
    ALLOCATE_SCALAR(SConst1);
    ALLOCATE_SCALAR(SSpikeWord);
    ALLOCATE_SCALAR(SISynBuffer);
    ALLOCATE_SCALAR(SStride);
    ALLOCATE_SCALAR(SRowBufferA);
    ALLOCATE_SCALAR(SRowBufferB);

    assert(numPre == 32);
    assert(numPost == 32);

    // Labels
    Label bitLoopStart;
    Label bitLoopBody;
    Label prefetchBody;
    Label zeroSpikeWord;
    Label zeroSpikePrefetchWord;
    Label wordEnd;
    Label waitOnDMA;

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

    // Load row buffer pointers
    c.li(*SRowBufferA, rowBufferAPtr);
    c.li(*SRowBufferB, rowBufferBPtr);
    
    // Load some useful constants
    c.li(*SConst1, 1);

    // SWordNStart = 31
    c.li(*SWordNStart, 31);

    c.li(*SStride, stride);

    // Zero isyn
    {
        ALLOCATE_VECTOR(VTmp);
        c.vlui(*VTmp, 0);
        c.vstore(*VTmp, *SISynBuffer);
    }
        
    // Register allocation
    ALLOCATE_SCALAR(SN);
    ALLOCATE_SCALAR(SNumLZ);
    ALLOCATE_SCALAR(SWeightBuffer);

    // SSpikeWord = *SSpikeBuffer++
    c.lw(*SSpikeWord, *SSpikeBuffer);

    // If SSpikeWord == 0, goto bitloop end
    c.beq(*SSpikeWord, Reg::X0, wordEnd);

    // SN = SWordNStart
    c.mv(*SN, *SWordNStart);

    // CNumLZ = clz(SSpikeWord);
    c.clz(*SNumLZ, *SSpikeWord);

    // If SSpikeWord == 1  i.e. CNumLZ == 31, goto zeroSpikeWord
    c.beq(*SSpikeWord, *SConst1, zeroSpikePrefetchWord);
        
    {
        // STmp = CNumLZ + 1
        ALLOCATE_SCALAR(STmp);
        c.addi(*STmp, *SNumLZ, 1);

        // SSpikeWord <<= CNumLZPlusOne
        c.sll(*SSpikeWord, *SSpikeWord, *STmp);
    }

    // SN -= SNumLZ
    c.L(prefetchBody);
    c.sub(*SN, *SN, *SNumLZ);

    // SWeightBuffer = weightInHidStart + (numPostVecs * 64 * SN);
    // **TODO** multiply
    c.li(*SWeightBuffer, weightPtr);
    {
        ALLOCATE_SCALAR(STemp);
        c.mul(*STemp, *SN, *SStride);
        c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
    }
    
    // Start DMA write into RowBufferA
    AssemblerUtils::generateDMAStartWrite(c, *SRowBufferA, *SWeightBuffer, *SStride);

    // SN --
    c.addi(*SN, *SN, -1);
       
    // If we've now got no spikes to process skip next DMA
    c.beq(*SSpikeWord, Reg::X0, waitOnDMA);

    // Inner bit loop
    c.L(bitLoopStart);
    {
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
        // **TODO** multiply
        ALLOCATE_SCALAR(SWeightBuffer);
        c.li(*SWeightBuffer, weightPtr);
        {
            ALLOCATE_SCALAR(STemp);
            c.li(*STemp, stride);
            c.mul(*STemp, *SN, *STemp);
            c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
        }

        // waitOnDMA
        c.L(waitOnDMA);

        // Wait for previous DMA write to completet
        AssemblerUtils::generateDMAWaitForWriteComplete(c, scalarRegisterAllocator);

        // Start DMA write into RowBufferB
        AssemblerUtils::generateDMAStartWrite(c, *SRowBufferB, *SWeightBuffer, *SStride);
        
        // SN --
        c.addi(*SN, *SN, -1);
            
        // Load weight and Isyn
        if(debug) {
            c.ebreak();
        }

        {
            ALLOCATE_VECTOR(VWeight);
            ALLOCATE_VECTOR(VISyn);
         
            // Load next vector of weights from row buffer
            c.vloadv(*VWeight, *SRowBufferA);
            c.vloadv(*VISyn, *SISynBuffer);

            // **STALL**
            c.nop();

            // Add weights to ISyn with mask
            c.vadd(*VISyn, *VISyn, *VWeight);
            
            // Write back ISyn
            c.vstore(*VISyn, *SISynBuffer);
        }

        // Swap buffers
        {
            ALLOCATE_SCALAR(STmp);
            c.mv(*STmp, *SRowBufferA);
            c.mv(*SRowBufferA, *SRowBufferB);
            c.mv(*SRowBufferB, *STmp);
        }
            
        // If SSpikeWord != 0, goto bitLoopStart
        c.bne(*SSpikeWord, Reg::X0, bitLoopStart);

        // Goto wordEnd
        //c.j_(wordEnd);
        c.beq(Reg::X0, Reg::X0, wordEnd);
    }

    // Zero spike word
    {
        c.L(zeroSpikePrefetchWord);
        c.li(*SSpikeWord, 0);
        //c.j_(bitLoopBody);
        c.beq(Reg::X0, Reg::X0, prefetchBody);
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

int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    bool device = false;

    CLI::App app{"Spike test with weights in DRAM"};
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");
    
    CLI11_PARSE(app, argc, argv);

    // Allocate memory
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Constants
    constexpr uint32_t numInput = 32;
    constexpr uint32_t numHidden = 32;
    constexpr uint32_t numInputSpikeWords = ceilDivide(numInput, 32);
    constexpr uint32_t numHiddenSpikeWords = ceilDivide(numHidden, 32);
    
    // Allocate vector arrays
    const uint32_t hiddenIsynPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t rowBufferAPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t rowBufferBPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    
    // Allocate scalar arrays
    const uint32_t inputSpikePtr = AppUtils::allocateScalarAndZero(numInputSpikeWords * 4, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Generate weight data pattern
    std::vector<int16_t> weights(numInput * numHiddenSpikeWords * 32);
    for(size_t i = 0; i < numInput; i++) {
        for(size_t j = 0; j < numHidden; j++) {
            weights[(i * numHiddenSpikeWords * 32) + j] = (32 * i) + j; 
        }
    }

    // Generate spikes
    {
        std::vector<uint32_t> test(numInputSpikeWords);
        for(size_t i = 0; i < numInputSpikeWords; i++) {
            test[i] = 0xDEADBEEF;
        }
        std::memcpy(scalarInitData.data() + inputSpikePtr, test.data(), test.size() * 4);
    }

    // Generate sim code
    const auto simCode = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {            
            // 2^6 = 2 bytes * 32 hidden neurons
            genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator,
                           0, inputSpikePtr, hiddenIsynPtr, rowBufferAPtr, rowBufferBPtr,
                           numInput, numHidden, numHiddenSpikeWords * 64, false);

        });
    
    if(device) {
        LOGI << "Creating device";
        Device device;

        // Create DMA buffer
        DMABuffer dmaBuffer;

        // Check there's enough space for 2 copies of transfers
        assert(dmaBuffer.getSize() >= (2 * weights.size()));

        // Get pointer to DMA buffer and copy in weights
        int16_t *bufferData = reinterpret_cast<int16_t*>(dmaBuffer.getData());
        std::copy(weights.cbegin(), weights.cend(), bufferData);

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
        
        // Start DMA to copy ISyn from URAM into DMA buffer
        device.getDMAController()->startRead(dmaBuffer, weights.size() * 2,
                                             hiddenIsynPtr, numHiddenSpikeWords * 64);
        device.getDMAController()->waitForReadComplete();

        // Check hidden ISyn directly in DMA buffer
        check(bufferData + weights.size(), numInput, numHidden);
    }
    else {
        RISCV riscV;
        riscV.setInstructions(simCode);
        riscV.getScalarDataMemory().setData(scalarInitData);
        
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);
        riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().setData(vectorInitData);
        
        // Create simulated DMA controller
        DMAControllerSim dmaController(riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory());
        riscV.setDMAController(&dmaController);

        // Check there's enough space for 2 copies of transfers
        assert(dmaController.getDataSize() >= (2 * weights.size()));

        // Get pointer to simulated DMA controller buffer and copy in weights
        int16_t *bufferData = reinterpret_cast<int16_t*>(dmaController.getData());
        std::copy(weights.cbegin(), weights.cend(), bufferData);


        if(!riscV.run()) {
            return 1;
        }

        // Check hidden ISyn directly in vector memory
        const int16_t *vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData();
        check(vectorData + hiddenIsynPtr, numInput, numHidden);
    }
 
    return 0;
}
