// Standard C++ includes
#include <bitset>
#include <chrono>
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
#include "ise/riscv.h"
#include "ise/vector_processor.h"

void genStaticPulse(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                    ScalarRegisterAllocator &scalarRegisterAllocator, uint32_t weightPtr, 
                    std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> preSpikePtr, uint32_t postISynPtr, 
                    uint32_t numPre, uint32_t numPost, uint32_t scaleShift, bool debug)
{
    // Register allocation
    ALLOCATE_SCALAR(SSpikeBufferEnd);
    ALLOCATE_SCALAR(SWordNStart);
    ALLOCATE_SCALAR(SConst1);
    ALLOCATE_SCALAR(SSpikeWord);
    ALLOCATE_SCALAR(SISynBuffer);

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
    c.li(*SISynBuffer, postISynPtr);

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

            ALLOCATE_VECTOR(VWeight);
            ALLOCATE_VECTOR(VISyn1);
            ALLOCATE_VECTOR(VISyn2);
            ALLOCATE_VECTOR(VISynNew);

            // Preload first ISyn to avoid stall
            c.vloadv(*VISyn1, *SISynBuffer, 0);

            AssemblerUtils::unrollVectorLoopBody(
                c, scalarRegisterAllocator, numPost, 4, *SISynBuffer,
                [SWeightBuffer, SISynBuffer, VWeight, VISyn1, VISyn2, VISynNew]
                (CodeGenerator &c, uint32_t r, bool even, ScalarRegisterAllocator::RegisterPtr maskReg)
                {
                    // Load vector of weights
                    c.vloadv(*VWeight, *SWeightBuffer, r * 64);

                    // Unless this is last unroll, load NEXT vector of ISyn to avoid stall
                    // **YUCK** in last iteration, while this may not be accessed, it may be out of 
                    c.vloadv(even ? *VISyn2 : *VISyn1, *SISynBuffer, (r + 1) * 64);
                    
                    // Add weights to ISyn
                    auto VISyn = even ? VISyn1 : VISyn2;

                    if(maskReg) {
                        c.vadd_s(*VISynNew, *VISyn, *VWeight);
                        c.vsel(*VISyn, *maskReg, *VISynNew);
                    }
                    else {
                        c.vadd_s(*VISyn, *VISyn, *VWeight);
                    }

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
}

std::vector<uint32_t> generateSimCode(bool simulate, uint32_t numInput, uint32_t numHidden, uint32_t numOutput, uint32_t numTimesteps,
                                      uint32_t hiddenFixedPoint, uint32_t outFixedPoint, uint32_t numInputSpikeWords,
                                      uint32_t numHiddenSpikeWords,  uint32_t weightInHidPtr, uint32_t weightHidOutPtr, 
                                      uint32_t outputBiasPtr, uint32_t hiddenIsynPtr, uint32_t hiddenVPtr, uint32_t hiddenRefracTimePtr, 
                                      uint32_t outputIsynPtr , uint32_t outputVPtr, uint32_t outputVSumPtr,
                                      uint32_t inputSpikeArrayPtr, uint32_t hiddenSpikePtr, uint32_t outputVSumScalarPtr, uint32_t readyFlagPtr)
{
    return AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Register allocation
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeEnd);

            // Labels
            Label timeLoop;
            Label spinLoop;

            // Set timestep range and load ready flag pointer
            c.li(*STime, 0);
            c.li(*STimeEnd, numTimesteps);

            // Loop over time
            c.L(timeLoop);
            {

                // ---------------------------------------------------------------
                // Input->Hidden synapses
                // ---------------------------------------------------------------
                {
                    ALLOCATE_SCALAR(SSpikeArrayBuffer);

                    // SSpikeArrayBuffer = inputSpikeArrayPtr + (100 * STime)
                    c.li(*SSpikeArrayBuffer, inputSpikeArrayPtr);
                    {
                        // STemp = number of bytes of full vector
                        ALLOCATE_SCALAR(STmp1);
                        ALLOCATE_SCALAR(STmp2);

                        // **TODO** single cycle DSP multiply!

                        // STime * 64
                        c.slli(*STmp1, *STime, 6);
                    
                        // STime * 32
                        c.slli(*STmp2, *STime, 5);

                        // STime * 96
                        c.add(*STmp1, *STmp1, *STmp2);

                        // STime * 4
                        c.slli(*STmp2, *STime, 2);

                        // STime * 100!
                        c.add(*STmp1, *STmp1, *STmp2);

                        // SSpikeTimeBufferEnd += STime * 100
                        c.add(*SSpikeArrayBuffer, *SSpikeArrayBuffer, *STmp1);
                    }

                    // 2^8 = 2 bytes * 128 hidden neurons
                    genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                    weightInHidPtr, SSpikeArrayBuffer, 
                                    hiddenIsynPtr, numInput, numHidden, 8, false);
                }

                // ---------------------------------------------------------------
                // Hidden->Output synapses
                // ---------------------------------------------------------------
                // 2^6 = 2 bytes * 32 output neurons (rounded up)
                genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator, 
                                weightHidOutPtr, hiddenSpikePtr, 
                                outputIsynPtr, numHidden, numOutput, 6, false);

                
                // ---------------------------------------------------------------
                // Hidden neurons
                // ---------------------------------------------------------------
                {
                    // Register allocation
                    ALLOCATE_SCALAR(SVBuffer);
                    ALLOCATE_SCALAR(SVBufferEnd);
                    ALLOCATE_SCALAR(SISynBuffer);
                    ALLOCATE_SCALAR(SRefracTimeBuffer);
                    ALLOCATE_SCALAR(SSpikeBuffer);
                    ALLOCATE_VECTOR(VAlpha);
                    ALLOCATE_VECTOR(VThresh);
                    ALLOCATE_VECTOR(VMinusThresh);
                    ALLOCATE_VECTOR(VTauRefrac);
                    ALLOCATE_VECTOR(VMinusDT);
                    ALLOCATE_VECTOR(VZero); 

                    // Load constants
                    // alpha = e^(-1/20)
                    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / 20.0), hiddenFixedPoint));
                    c.vlui(*VThresh, convertFixedPoint(0.61, hiddenFixedPoint));
                    c.vlui(*VMinusThresh, (uint16_t)convertFixedPoint(-0.61, hiddenFixedPoint));
                    c.vlui(*VTauRefrac, 5);
                    c.vlui(*VMinusDT, (uint16_t)-1);
                    c.vlui(*VZero, 0);

                    // Get address of buffers
                    c.li(*SVBuffer, hiddenVPtr);
                    c.li(*SISynBuffer, hiddenIsynPtr);
                    c.li(*SRefracTimeBuffer, hiddenRefracTimePtr);
                    c.li(*SSpikeBuffer, hiddenSpikePtr);
                 
                    AssemblerUtils::unrollVectorLoopBody(
                        c, scalarRegisterAllocator, numHidden, 4, *SVBuffer,
                        [&scalarRegisterAllocator, &vectorRegisterAllocator,
                         hiddenFixedPoint, 
                         SVBuffer, SISynBuffer, SRefracTimeBuffer, SSpikeBuffer,
                         VAlpha, VMinusDT, VTauRefrac, VThresh, VMinusThresh, VZero]
                        (CodeGenerator &c, uint32_t r, bool, ScalarRegisterAllocator::RegisterPtr maskReg)
                        {
                            assert(!maskReg);

                            // Register allocation
                            ALLOCATE_VECTOR(VV);
                            ALLOCATE_VECTOR(VISyn);
                            ALLOCATE_VECTOR(VRefracTime);
                            ALLOCATE_SCALAR(SSpikeOut);
                            ALLOCATE_SCALAR(SRefractory); 
                            
                            // Load voltage and isyn
                            c.vloadv(*VV, *SVBuffer, 64 * r);
                            c.vloadv(*VISyn, *SISynBuffer, 64 * r);
                            c.vloadv(*VRefracTime, *SRefracTimeBuffer, 64 * r);

                            // VV *= VAlpha
                            c.vmul(hiddenFixedPoint, *VV, *VV, *VAlpha);

                            // VV += VISyn
                            c.vadd_s(*VV, *VV, *VISyn);

                            // VISyn = 0
                            c.vlui(*VISyn, 0);

                            // SRefractory = VRefracTime > 0.0 (0.0 < VRefracTime)
                            c.vtlt(*SRefractory, *VZero, *VRefracTime);
                            {
                                // VTemp = VRefracTime - VDT
                                ALLOCATE_VECTOR(VTemp);
                                //c.vsub(*VTemp, *VRefracTime, *VDT);
                                c.vadd(*VTemp, *VRefracTime, *VMinusDT);
                            
                                // VRefracTime = SRefractory ? VTemp : VRefracTime
                                c.vsel(*VRefracTime, *SRefractory, *VTemp);
                            }

                            // SSpikeOut = VV >= VThresh && !SRefractory
                            c.vtge(*SSpikeOut, *VV, *VThresh);
                            {
                                // STemp = !SRefractory
                                ALLOCATE_SCALAR(STemp);
                                c.not_(*STemp, *SRefractory);
                                c.and_(*SSpikeOut, *SSpikeOut, *STemp);
                            }

                            // *SSpikeBuffer = SSpikeOut
                            c.sw(*SSpikeOut, *SSpikeBuffer, 4 * r);

                            {
                                // VTemp = V - VThresh
                                ALLOCATE_VECTOR(VTemp);
                                c.vadd(*VTemp, *VV, *VMinusThresh);
                            
                                // VV = SSpikeOut ? VReset : VV
                                c.vsel(*VV, *SSpikeOut, *VTemp);
                            }

                            // VRefracTime = SSpikeOut ? VTauRefrac : VRefracTime
                            c.vsel(*VRefracTime, *SSpikeOut, *VTauRefrac);

                            // Store VV, ISyn and refrac time and increment buffers
                            c.vstore(*VV, *SVBuffer, 64 * r);
                            c.vstore(*VISyn, *SISynBuffer, 64 * r);
                            c.vstore(*VRefracTime, *SRefracTimeBuffer, 64 * r);
                        },
                        [SISynBuffer, SRefracTimeBuffer, SSpikeBuffer, SVBuffer]
                        (CodeGenerator &c, uint32_t numUnrolls)
                        {
                            c.addi(*SVBuffer, *SVBuffer, 64 * numUnrolls);
                            c.addi(*SISynBuffer, *SISynBuffer, 64 * numUnrolls);
                            c.addi(*SRefracTimeBuffer, *SRefracTimeBuffer, 64 * numUnrolls);
                            c.addi(*SSpikeBuffer, *SSpikeBuffer, 4 * numUnrolls); 
                        });
                }

                // ---------------------------------------------------------------
                // Output neurons
                // ---------------------------------------------------------------
                {
                    // Register allocation
                    ALLOCATE_SCALAR(SVBuffer);
                    ALLOCATE_SCALAR(SVSumBuffer);
                    ALLOCATE_SCALAR(SISynBuffer);
                    ALLOCATE_SCALAR(SBiasBuffer);
                    ALLOCATE_VECTOR(VAlpha);
                    ALLOCATE_VECTOR(VZero);

                    // Load constants
                    // alpha = e^(-1/20)
                    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / 20.0), outFixedPoint));
                    c.vlui(*VZero, 0);

                    // Get address of voltage, voltage sum and Isyn buffers
                    c.li(*SVBuffer, outputVPtr);
                    c.li(*SVSumBuffer, outputVSumPtr);
                    c.li(*SISynBuffer, outputIsynPtr);
                    c.li(*SBiasBuffer, outputBiasPtr);

                    // Output neuron tail
                    {
                        // Register allocation
                        ALLOCATE_SCALAR(SMask);
                        ALLOCATE_VECTOR(VV);
                        ALLOCATE_VECTOR(VVNew);
                        ALLOCATE_VECTOR(VVSum);
                        ALLOCATE_VECTOR(VVSumNew);
                        ALLOCATE_VECTOR(VISyn);
                        ALLOCATE_VECTOR(VBias);
                    
                        // Calculate mask
                        c.li(*SMask, (1 << (32 - numOutput)) - 1);

                        // Load V, VSum and ISyn
                        c.vloadv(*VV, *SVBuffer);
                        c.vloadv(*VVSum, *SVSumBuffer);
                        c.vloadv(*VISyn, *SISynBuffer);
                        c.vloadv(*VBias, *SBiasBuffer);

                        // VV *= VAlpha
                        c.vmul(outFixedPoint, *VVNew, *VV, *VAlpha);

                        // VV += VISyn
                        c.vadd_s(*VVNew, *VVNew, *VISyn);

                        // VV += VBias
                        c.vadd_s(*VVNew, *VVNew, *VBias);

                        // VSum += VV
                        c.vadd_s(*VVSumNew, *VVSum, *VISyn);

                        // Zero Isyn
                        c.vlui(*VISyn, 0);

                        // Update original registers with mask
                        c.vsel(*VV, *SMask, *VVNew);
                        c.vsel(*VVSum, *SMask, *VVSumNew);
                    
                        // Store V, VSum and ISyn
                        c.vstore(*VV, *SVBuffer);
                        c.vstore(*VVSum, *SVSumBuffer);
                        c.vstore(*VISyn, *SISynBuffer);
                    }
                }

                c.addi(*STime, *STime, 1);
                c.bne(*STime, *STimeEnd, timeLoop);
            }

            // Copy output v sum to scalar memory and zero
            {
                // Register allocation
                ALLOCATE_VECTOR(VVSum);
                ALLOCATE_SCALAR(SVSumBuffer);
                ALLOCATE_SCALAR(SVSumScalarBuffer);

                // Get address of voltage sum buffer in vector memory and load
                assert(numOutput < 32);
                c.li(*SVSumBuffer, outputVSumPtr);
                c.vloadv(*VVSum, *SVSumBuffer, 0);

                // Get address of sum bufffer in scalar memory
                c.li(*SVSumScalarBuffer, outputVSumScalarPtr);

                // Unroll lane loop
                for(uint32_t l = 0; l < numOutput; l++) {
                    // Register allocation
                    ALLOCATE_SCALAR(SVal);
            
                    // Extract lane into scalar registers
                    c.vextract(*SVal, *VVSum, l);

                    // Store halfword
                    c.sh(*SVal, *SVSumScalarBuffer, l * 2);
                }

                // Zero vector and write back
                c.vlui(*VVSum, 0);
                c.vstore(*VVSum, *SVSumBuffer, 0);

            }
        });
}

int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    bool device = false;
    size_t numExamples = 10000;

    CLI::App app{"Latency MNIST inference"};
    app.add_option("-n,--num-examples", numExamples, "How many examples to simulate");
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");
    

    CLI11_PARSE(app, argc, argv);
    
    // Allocate memory
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Constants
    constexpr uint32_t numInput = 28 * 28;
    constexpr uint32_t numHidden = 128;
    constexpr uint32_t numOutput = 10;
    constexpr uint32_t numTimesteps = 79;
    constexpr uint32_t hiddenFixedPoint = 5;
    constexpr uint32_t outFixedPoint = 6;
    constexpr uint32_t numInputSpikeWords = ceilDivide(numInput, 32);
    constexpr uint32_t numHiddenSpikeWords = ceilDivide(numHidden, 32);
    constexpr uint32_t numOutputSpikeWords = ceilDivide(numOutput, 32);
    constexpr uint32_t numInputSpikeArrayWords = numInputSpikeWords * numTimesteps;

    // Allocate vector arrays
    const uint32_t weightInHidPtr = AppUtils::loadVectors("mnist_in_hid.bin", vectorInitData);
    const uint32_t weightHidOutPtr = AppUtils::loadVectors("mnist_hid_out.bin", vectorInitData);
    const uint32_t outputBiasPtr = AppUtils::loadVectors("mnist_bias.bin", vectorInitData);
    
    const uint32_t hiddenIsynPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t hiddenVPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t hiddenRefracTimePtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);

    const uint32_t outputIsynPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
    const uint32_t outputVPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
    const uint32_t outputVSumPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
   
    // Allocate scalar arrays
    const uint32_t inputSpikeArrayPtr = AppUtils::allocateScalarAndZero(numInputSpikeArrayWords * 4, scalarInitData);
    const uint32_t hiddenSpikePtr = AppUtils::allocateScalarAndZero(numHiddenSpikeWords * 4, scalarInitData);
    const uint32_t outputVSumScalarPtr = AppUtils::allocateScalarAndZero(numOutput * 2, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Load data
    const auto mnistSpikes = AppUtils::loadBinaryData<uint32_t>("mnist_spikes.bin");
    const auto mnistLabels = AppUtils::loadBinaryData<int16_t>("mnist_labels.bin");
   
    // Generate sim code
    const auto simCode = generateSimCode(!device, numInput, numHidden, numOutput, numTimesteps,
                                         hiddenFixedPoint, outFixedPoint, numInputSpikeWords,
                                         numHiddenSpikeWords,  weightInHidPtr, weightHidOutPtr, 
                                         outputBiasPtr, hiddenIsynPtr, hiddenVPtr,
                                         hiddenRefracTimePtr, outputIsynPtr, outputVPtr, outputVSumPtr,
                                         inputSpikeArrayPtr, hiddenSpikePtr, outputVSumScalarPtr, readyFlagPtr);
    LOGI << simCode.size() << " simulation instructions";
    LOGI << scalarInitData.size() << " bytes of scalar memory required";
    LOGI << vectorInitData.size() * 2 << " bytes of vector memory required (" << ceilDivide(vectorInitData.size() / 32, 4096) << " URAM cascade)";

    if(device) {
        LOGI << "Creating device";
        Device device;

        // Put core into reset state
        LOGI << "Resetting";
        device.setEnabled(false);
        
        {
            LOGI << "DMAing vector init data to device";
           
            // Create DMA buffer
            DMABuffer dmaBuffer;

            // Check there's enough space for vector init data
            assert(dmaBuffer.getSize() > (vectorInitData.size() * 2));

            // Get halfword pointer to DMA buffer
            int16_t *bufferData = reinterpret_cast<int16_t*>(dmaBuffer.getData());
            
            // Copy vector init data to buffer
            std::copy(vectorInitData.cbegin(), vectorInitData.cend(), bufferData);
            
            for(int i = 0; i < vectorInitData.size() * 2; i += 8192) {
                // Start DMA of data to URAM
                device.getDMAController()->startWrite(i, dmaBuffer, i, std::min(8192ull, (vectorInitData.size() * 2) - 8192ull));
    
                // Wait for write to complete
                device.getDMAController()->waitForWriteComplete();
            }
        }
        
        // Simulation
        {
            LOGI << "Copying simulation instructions (" << simCode.size() * sizeof(uint32_t) << " bytes)";
            device.uploadCode(simCode);

            // Loop through examples
            int numCorrect = 0;
            const volatile int16_t *outputVSum = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + outputVSumScalarPtr);
            std::chrono::duration<double> duration{0};
            for(size_t i = 0; i < numExamples; i++) {
                // Copy input spike bits into scalar memory
                device.memcpyDataToDevice(inputSpikeArrayPtr, 
                                          reinterpret_cast<const uint8_t*>(mnistSpikes.data() + (numInputSpikeArrayWords * i)),
                                          numInputSpikeArrayWords * 4);

                // Put core into running state and trigger ILA
                device.setILATrigger(true);
                device.setEnabled(true);
                const auto startTime = std::chrono::high_resolution_clock::now();

                // Wait until ready flag
                device.waitOnNonZero(readyFlagPtr); 
                duration += (std::chrono::high_resolution_clock::now() - startTime);

                // Reset core
                device.setEnabled(false);
                device.setILATrigger(false);

                // Determine if output is correct
                const auto classification = std::distance(outputVSum, std::max_element(outputVSum, outputVSum + 10));
                if(classification == mnistLabels[i]) {
                    numCorrect++;
                }
            }

            std::cout << numCorrect << " / " << numExamples << " correct (" << 100.0 * (numCorrect / double(numExamples)) << "%)" << std::endl;
            std::cout << duration.count() << " seconds" << std::endl;
        }
    }
    else {
        RISCV riscV(simCode, scalarInitData);
        
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);

        // Get pointer to output sim
        auto *scalarData = riscV.getScalarDataMemory().getData().data();
        const int16_t *outputVSum = reinterpret_cast<const int16_t*>(scalarData + outputVSumScalarPtr);

        // Loop through examples
        int numCorrect = 0;
        for(size_t i = 0; i < numExamples; i++) {
            // Copy input spike bits into scalar memory
            std::copy_n(mnistSpikes.data() + (numInputSpikeArrayWords * i), numInputSpikeArrayWords, 
                        reinterpret_cast<uint32_t*>(scalarData + inputSpikeArrayPtr));

            // Reset PC and run
            riscV.setPC(0);
            if(!riscV.run()) {
                return 1;
            }

            // Determine if output is correct
            const auto classification = std::distance(outputVSum, std::max_element(outputVSum, outputVSum + 10));
            if(classification == mnistLabels[i]) {
                numCorrect++;
            }
        }

        std::cout << numCorrect << " / " << numExamples << " correct (" << 100.0 * (numCorrect / double(numExamples)) << "%)" << std::endl;
        std::cout << "Stats:" << std::endl;
        std::cout << "\t" << riscV.getTotalNumInstructionsExecuted() << " instructions executed" << std::endl;
        std::cout << "\t\t" << riscV.getTotalNumCoprocessorInstructionsExecuted(vectorQuadrant) << " vector instructions executed" << std::endl;
        std::cout << "\t\t" << riscV.getNumJumps() << " jumps" << std::endl;
        std::cout << "\t\t" << riscV.getNumMemory() << " scalar memory" << std::endl;
        std::cout << "\t\t" << riscV.getNumALU() << " scalar ALU" << std::endl;
        std::cout << "\t\t" << riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getNumMemory(riscV.getNumCoprocessorInstructionsExecuted(vectorQuadrant)) << " vector memory" << std::endl;
        std::cout << "\t\t" << riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getNumALU(riscV.getNumCoprocessorInstructionsExecuted(vectorQuadrant)) << " vector ALU" << std::endl;
    }

    return 0;
}
