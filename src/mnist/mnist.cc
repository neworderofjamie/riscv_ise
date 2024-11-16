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

#define RECORD_HIDDEN_SPIKES
//#define RECORD_HIDDEN_ISYN
//#define RECORD_HIDDEN_V
//#define RECORD_OUTPUT_ISYN
//#define RECORD_OUTPUT_V

void genStaticPulse(CodeGenerator &c, RegisterAllocator<VReg> &vectorRegisterAllocator,
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
    constexpr uint32_t numInput = 28 * 28;
    constexpr uint32_t numHidden = 128;
    constexpr uint32_t numOutput = 10;
    constexpr uint32_t numTimesteps = 79;
    constexpr uint32_t hiddenFixedPoint = 5;
    constexpr uint32_t outFixedPoint = 6;
    constexpr size_t numExamples = 1;
    constexpr uint32_t numInputSpikeWords = ceilDivide(numInput, 32);
    constexpr uint32_t numHiddenSpikeWords = ceilDivide(numHidden, 32);
    constexpr uint32_t numOutputSpikeWords = ceilDivide(numOutput, 32);
    constexpr uint32_t numInputSpikeArrayWords = numInputSpikeWords * numTimesteps;
    constexpr uint32_t numHiddenSpikeArrayWords = numHiddenSpikeWords * numTimesteps;
    constexpr uint32_t numOutputSpikeArrayWords = numOutputSpikeWords * numTimesteps;

    // Allocate vector arrays
    // **NOTE** these are adjacent so data can be block-copied from scalar memory
    const uint32_t weightInHidPtr = AppUtils::allocateVectorAndZero(numInput * numHiddenSpikeWords * 32, vectorInitData);
    const uint32_t weightHidOutPtr = AppUtils::allocateVectorAndZero(numHidden * numOutputSpikeWords * 32, vectorInitData);
    const uint32_t outputBiasPtr = AppUtils::allocateVectorAndZero(numOutputSpikeWords * 32, vectorInitData);
    
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
#ifdef RECORD_HIDDEN_SPIKES
    const uint32_t hiddenSpikeRecordingPtr = AppUtils::allocateScalarAndZero(numHiddenSpikeArrayWords * 4, scalarInitData);
#endif
#ifdef RECORD_HIDDEN_VAR
    const uint32_t hiddenVarRecordingPtr = AppUtils::allocateScalarAndZero(numHiddenSpikeArrayWords * 64, scalarInitData);
#endif
#ifdef RECORD_OUTPUT_VAR
    const uint32_t outputVarRecordingPtr = AppUtils::allocateScalarAndZero(numOutputSpikeArrayWords * 64, scalarInitData);
#endif
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Increase scalar memory size
    scalarInitData.resize(64 * 1024, 0);

    // Load data
    const auto mnistSpikes = AppUtils::loadBinaryData<uint32_t>("mnist_spikes.bin");
    const auto mnistLabels = AppUtils::loadBinaryData<int16_t>("mnist_labels.bin");

    // Load weights
    const auto weightInHid = AppUtils::loadBinaryData<uint8_t>("mnist_in_hid.bin");
    const auto weightHidOut = AppUtils::loadBinaryData<uint8_t>("mnist_hid_out.bin");
    const auto outputBias = AppUtils::loadBinaryData<uint8_t>("mnist_bias.bin");

    // Check first two are correctly padded
    assert(weightInHid.size() == numInput * numHiddenSpikeWords * 64);
    assert(weightHidOut.size() == numHidden * numOutputSpikeWords * 64);
    
    // Concatenate into single vector
    std::vector<uint8_t> initData;
    initData.reserve(weightInHid.size() + weightHidOut.size() + outputBias.size());
    std::copy(weightInHid.cbegin(), weightInHid.cend(), std::back_inserter(initData));
    std::copy(weightHidOut.cbegin(), weightHidOut.cend(), std::back_inserter(initData));
    std::copy(outputBias.cbegin(), outputBias.cend(), std::back_inserter(initData));

    // Generate sim code
    const auto simCode = AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Register allocation
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeEnd);
#ifdef RECORD_HIDDEN_SPIKES
            ALLOCATE_SCALAR(SHiddenSpikeRecordingBuffer);
#endif
#ifdef RECORD_HIDDEN_VAR
            ALLOCATE_SCALAR(SHiddenVarRecordingBuffer);
#endif
#ifdef RECORD_OUTPUT_VAR
            ALLOCATE_SCALAR(SOutputVarRecordingBuffer);
#endif
            // Labels
            Label timeLoop;
            Label spinLoop;

            // Set timestep range and load ready flag pointer
            c.li(*STime, 0);
            c.li(*STimeEnd, numTimesteps);
#ifdef RECORD_HIDDEN_SPIKES
            c.li(*SHiddenSpikeRecordingBuffer, hiddenSpikeRecordingPtr);
#endif
#ifdef RECORD_HIDDEN_VAR
            c.li(*SHiddenVarRecordingBuffer, hiddenVarRecordingPtr);
#endif
#ifdef RECORD_OUTPUT_VAR
            c.li(*SOutputVarRecordingBuffer, outputVarRecordingPtr);
#endif
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
                    ALLOCATE_VECTOR(VTauRefrac);
                    ALLOCATE_VECTOR(VDT);
                    ALLOCATE_VECTOR(VZero); 

                    // Load constants
                    // alpha = e^(-1/20)
                    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / 20.0), hiddenFixedPoint));
                    c.vlui(*VThresh, convertFixedPoint(0.61, hiddenFixedPoint));
                    c.vlui(*VTauRefrac, 5);
                    c.vlui(*VDT, 1);
                    c.vlui(*VZero, 0);

                    // Get address of buffers
                    c.li(*SVBuffer, hiddenVPtr);
                    c.li(*SVBufferEnd, hiddenVPtr + (numHiddenSpikeWords * 64));
                    c.li(*SISynBuffer, hiddenIsynPtr);
                    c.li(*SRefracTimeBuffer, hiddenRefracTimePtr);
                    c.li(*SSpikeBuffer, hiddenSpikePtr);
                 
                    AssemblerUtils::unrollVectorLoopBody(
                        c, numHidden, 4, *SVBuffer, *SVBufferEnd,
                        [&scalarRegisterAllocator, &vectorRegisterAllocator,
                         hiddenFixedPoint, 
                         SVBuffer, SISynBuffer, SRefracTimeBuffer, SSpikeBuffer,
 #ifdef RECORD_HIDDEN_SPIKES
                         SHiddenSpikeRecordingBuffer,
#endif
#ifdef RECORD_HIDDEN_VAR
                         SHiddenVarRecordingBuffer,
#endif
                         VAlpha, VDT, VTauRefrac, VThresh, VZero]
                        (CodeGenerator &c, uint32_t r)
                        {
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
                            c.vadd(*VV, *VV, *VISyn);
#ifdef RECORD_HIDDEN_VAR
                            ALLOCATE_SCALAR(STmp);
                            for(int l = 0; l < 32; l++) {
                                c.vextract(*STmp, *VV, l);
                                c.sh(*STmp, *SHiddenVarRecordingBuffer, l * 2);
                            }
                            c.addi(*SHiddenVarRecordingBuffer, *SHiddenVarRecordingBuffer, 64);
#endif
                            // VISyn = 0
                            c.vlui(*VISyn, 0);

                            // SRefractory = VRefracTime > 0.0 (0.0 < VRefracTime)
                            c.vtlt(*SRefractory, *VZero, *VRefracTime);
                            {
                                // VTemp = VRefracTime - VDT
                                ALLOCATE_VECTOR(VTemp);
                                c.vsub(*VTemp, *VRefracTime, *VDT);
                            
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
#ifdef RECORD_HIDDEN_SPIKES
                            c.sw(*SSpikeOut, *SHiddenSpikeRecordingBuffer);
                            c.addi(*SHiddenSpikeRecordingBuffer, *SHiddenSpikeRecordingBuffer, 4);
#endif
                            {
                                // VTemp = V - VThresh
                                ALLOCATE_VECTOR(VTemp);
                                c.vsub(*VTemp, *VV, *VThresh);
                            
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
                        c.vadd(*VVNew, *VVNew, *VISyn);

                        // VV += VBias
                        c.vadd(*VVNew, *VVNew, *VBias);

                        // VSum += VV
                        c.vadd(*VVSumNew, *VVSum, *VISyn);
#ifdef RECORD_OUTPUT_VAR
                        for(int l = 0; l < numOutput; l++) {
                            c.vextract(*STmp, *VISyn, l);
                            c.sh(*STmp, *SOutputVarRecordingBuffer, l * 2);
                        }
                        c.addi(*SOutputVarRecordingBuffer, *SOutputVarRecordingBuffer, numOutput * 2);
#endif
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
    LOGI << simCode.size() << " simulation instructions";
    LOGI << scalarInitData.size() << " bytes of scalar memory required";
    LOGI << vectorInitData.size() * 2 << " bytes of vector memory required (" << ceilDivide(vectorInitData.size() / 32, 4096) << " URAM cascade)";

    // Generate initialisation code to copy blocks of data from scalar to vector memory
    const uint32_t initStartVectorPtr = 0;
    const uint32_t initNumVectorsPtr = 4;
    const uint32_t initReadyFlagPtr = 8;
    const uint32_t initScalarScratchPtr = 12;
    const auto initCode = AssemblerUtils::generateInitCode(simulate, initStartVectorPtr, initNumVectorsPtr, 
                                                           initReadyFlagPtr, initScalarScratchPtr);
    
    if(simulate) {
        RISCV riscV(initCode, scalarInitData);
        
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
        
        // Run kernels to copy initData into vector memory
        if(!riscV.runInit(initData, initStartVectorPtr, initNumVectorsPtr, initScalarScratchPtr, weightInHidPtr)) {
            return 1;
        }
            
        // Reset stats for simulations
        riscV.resetStats();

        // Load simulation program
        riscV.setInstructions(simCode);

        // From these, get pointers to data structures
        // 
        //const uint32_t *inputSpikeWords = reinterpret_cast<const uint32_t*>(scalarData + inputSpikePtr);
        //const uint32_t *hiddenSpikeWords = reinterpret_cast<const uint32_t*>(scalarData + hiddenSpikePtr);
        auto *scalarData = riscV.getScalarDataMemory().getData().data();
        const int16_t *outputVSum = reinterpret_cast<const int16_t*>(scalarData + outputVSumScalarPtr);

        // Loop through examples
        int numCorrect = 0;
        for(size_t i = 0; i < numExamples; i++) {
            // Show % progress
            const auto iPerc = std::div(i, 100);
            if(iPerc.rem == 0) {
                std:: cout << iPerc.quot << "%" << std::endl;
            }

            // Copy input spike bits into scalar memory
            std::copy_n(mnistSpikes.data() + (numInputSpikeArrayWords * i), numInputSpikeArrayWords, 
                        reinterpret_cast<uint32_t*>(scalarData + inputSpikeArrayPtr));

            // Reset PC and run
            riscV.setPC(0);
            if(!riscV.run()) {
                return 1;
            }
#ifdef RECORD_HIDDEN_SPIKES
            const uint32_t *hiddenSpikeRecording = reinterpret_cast<const uint32_t*>(scalarData + hiddenSpikeRecordingPtr);
            std::ofstream spikeFile("mnist_spikes_sim.csv");
            for(size_t t = 0; t < numTimesteps; t++) {
                AppUtils::writeSpikes(spikeFile, hiddenSpikeRecording, t, numHiddenSpikeWords);
                hiddenSpikeRecording += numHiddenSpikeWords;
            }
#endif

#ifdef RECORD_HIDDEN_VAR
            const int16_t *hiddenVarRecording = reinterpret_cast<const int16_t*>(scalarData + hiddenVarRecordingPtr);
            std::ofstream isynFile("mnist_v_sim.csv");
            for(size_t t = 0; t < numTimesteps; t++) {
                for(size_t i = 0; i < numHidden; i++) {
                    isynFile << *hiddenVarRecording++;
                    if(i != (numHidden - 1)) {
                        isynFile << ", ";
                    }
                }
                isynFile << std::endl;
            }
#endif
#ifdef RECORD_OUTPUT_VAR
            const int16_t *outputVarRecording = reinterpret_cast<const int16_t*>(scalarData + outputVarRecordingPtr);
            std::ofstream isynFile("mnist_output_isyn_sim.csv");
            for(size_t t = 0; t < numTimesteps; t++) {
                for(size_t i = 0; i < numOutput; i++) {
                    isynFile << *outputVarRecording++;
                    if(i != (numOutput - 1)) {
                        isynFile << ", ";
                    }
                }
                isynFile << std::endl;
            }
#endif
            // Determine if output is correct
            const auto classification = std::distance(outputVSum, std::max_element(outputVSum, outputVSum + 10));
            if(classification == mnistLabels[i]) {
                numCorrect++;
            }
        }

        std::cout << numCorrect << " / 10000 correct (" << 100.0 * (numCorrect / 10000.0) << "%)" << std::endl;
        std::cout << "Stats:" << std::endl;
        std::cout << "\t" << riscV.getTotalNumInstructionsExecuted() << " instructions executed" << std::endl;
        std::cout << "\t\t" << riscV.getTotalNumCoprocessorInstructionsExecuted(vectorQuadrant) << " vector instructions executed" << std::endl;
        std::cout << "\t\t" << riscV.getNumJumps() << " jumps" << std::endl;
        std::cout << "\t\t" << riscV.getNumMemory() << " scalar memory" << std::endl;
        std::cout << "\t\t" << riscV.getNumALU() << " scalar ALU" << std::endl;
        std::cout << "\t\t" << riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getNumMemory(riscV.getNumCoprocessorInstructionsExecuted(vectorQuadrant)) << " vector memory" << std::endl;
        std::cout << "\t\t" << riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getNumALU(riscV.getNumCoprocessorInstructionsExecuted(vectorQuadrant)) << " vector ALU" << std::endl;
    }
    else {
        LOGI << "Creating device";
        Device device;

        // Put core into reset state
        LOGI << "Resetting";
        device.setEnabled(false);
        
        // Initialisation
        {
            LOGI << "Copying init instructions (" << initCode.size() * sizeof(uint32_t) << " bytes)";
            device.uploadCode(initCode);

            // Run kernels to copy initData into vector memory
            device.runInit(initData, initStartVectorPtr, initNumVectorsPtr, initScalarScratchPtr, weightInHidPtr, initReadyFlagPtr);
        }
        
        // Simulation
        {
            LOGI << "Copying simulation instructions (" << simCode.size() * sizeof(uint32_t) << " bytes)";
            device.uploadCode(simCode);

            // Loop through examples
            int numCorrect = 0;
            const volatile int16_t *outputVSum = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + outputVSumScalarPtr);
            for(size_t i = 0; i < numExamples; i++) {
                // Show % progress
                const auto iPerc = std::div(i, 100);
                if(iPerc.rem == 0) {
                    std:: cout << iPerc.quot << "%" << std::endl;
                }

                // Copy input spike bits into scalar memory
                device.memcpyDataToDevice(inputSpikeArrayPtr, 
                                          reinterpret_cast<const uint8_t*>(mnistSpikes.data() + (numInputSpikeArrayWords * i)),
                                          numInputSpikeArrayWords * 4);

                // Put core into running state and trigger ILA
                LOGI << "Enabling";
                device.setILATrigger(true);
                device.setEnabled(true);

                // Wait until ready flag
                device.waitOnNonZero(readyFlagPtr);

                // Reset core
                LOGI << "Disabling";
                device.setEnabled(false);
                device.setILATrigger(false);
#ifdef RECORD_HIDDEN_SPIKES
                const volatile uint32_t *hiddenSpikeRecording = reinterpret_cast<const volatile uint32_t*>(device.getDataMemory() + hiddenSpikeRecordingPtr);
                std::ofstream spikeFile("mnist_spikes_device.csv");
                for(size_t t = 0; t < numTimesteps; t++) {
                    AppUtils::writeSpikes(spikeFile, hiddenSpikeRecording, t, numHiddenSpikeWords);
                    hiddenSpikeRecording += numHiddenSpikeWords;
                }
#endif
             
#ifdef RECORD_HIDDEN_VAR
                const volatile int16_t *hiddenVarRecording = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + hiddenVarRecordingPtr);
                std::ofstream isynFile("mnist_v_device.csv");
                for(size_t t = 0; t < numTimesteps; t++) {
                    for(size_t i = 0; i < numHidden; i++) {
                        isynFile << *hiddenVarRecording++;
                        if(i != (numHidden - 1)) {
                            isynFile << ", ";
                        }
                    }
                    isynFile << std::endl;
                }
#endif
#ifdef RECORD_OUTPUT_VAR
                const volatile int16_t *outputVarRecording = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + outputVarRecordingPtr);
                std::ofstream isynFile("mnist_output_isyn_device.csv");
                for(size_t t = 0; t < numTimesteps; t++) {
                    for(size_t i = 0; i < numOutput; i++) {
                        isynFile << *outputVarRecording++;
                        if(i != (numOutput - 1)) {
                            isynFile << ", ";
                        }
                    }
                    isynFile << std::endl;
                }
#endif
                // Determine if output is correct
                const auto classification = std::distance(outputVSum, std::max_element(outputVSum, outputVSum + 10));
                if(classification == mnistLabels[i]) {
                    numCorrect++;
                }
            }

            std::cout << numCorrect << " / 10000 correct (" << 100.0 * (numCorrect / 10000.0) << "%)" << std::endl;
        }
    }
    return 0;
}
