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
#include "common/utils.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"

std::vector<int16_t> loadData(const std::string &filename)
{
    std::ifstream input(filename, std::ios::binary);

    // Get length
    input.seekg (0, std::ios::end);
    const auto lengthBytes = input.tellg();
    input.seekg (0, std::ios::beg);

    // Check contents is half-word aligned
    assert((lengthBytes & 1) == 0);

    // Create vector
    std::vector<int16_t> data(lengthBytes / 2, 0);

    // Read data directly into it
    input.read(reinterpret_cast<char*>(data.data()), lengthBytes);

    return data;
}

void genStaticPulse(CodeGenerator &c, RegisterAllocator<VReg> &vectorRegisterAllocator,
                    RegisterAllocator<Reg> &scalarRegisterAllocator,
                    uint32_t weightPtr, uint32_t preSpikePtr, uint32_t postISynPtr, 
                    uint32_t numPre, uint32_t numPost, uint32_t scaleShift, bool debug)
{
    // Register allocation
    ALLOCATE_SCALAR(SSpikeBuffer);
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
    Label weightLoop;
    Label bitLoopEnd;
    Label zeroSpikeWord;
    Label wordEnd;

    // Get address of start of presynaptic spike buffer
    c.li(*SSpikeBuffer, preSpikePtr);
    
    // Get address of end of presynaptic spike buffer
    c.li(*SSpikeBufferEnd, preSpikePtr + (ceilDivide(numPre, 32) * 4));

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
                ALLOCATE_VECTOR(VISyn);
                AppUtils::unrollVectorLoopBody(
                    c, numPost, 4, *SISynBuffer, *SISynBufferEnd,
                    [SWeightBuffer, SISynBuffer, VWeight, VISyn]
                    (CodeGenerator &c, uint32_t r)
                    {
                        // Load next vector of weights and ISyns
                        c.vloadv(*VWeight, *SWeightBuffer, r * 64);
                        c.vloadv(*VISyn, *SISynBuffer, r * 64);
            

                        // Add weights to ISyn
                        c.vadd(*VISyn, *VISyn, *VWeight);

                        // Write back ISyn and increment SISynBuffer
                        c.vstore(*VISyn, *SISynBuffer, r * 64);
                    },
                    [SWeightBuffer, SISynBuffer, VWeight, VISyn]
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
    constexpr uint32_t numInput = 28 * 28;
    constexpr uint32_t numHidden = 128;
    constexpr uint32_t numOutput = 10;
    constexpr uint32_t hiddenFixedPoint = 5;
    constexpr uint32_t outFixedPoint = 6;
    constexpr uint32_t numInputSpikeWords = ceilDivide(numInput, 32);
    constexpr uint32_t numHiddenSpikeWords = ceilDivide(numHidden, 32);

    // Load vector data
    const uint32_t weightInHidPtr = AppUtils::loadVectors("mnist_in_hid.bin", vectorInitData);
    const uint32_t weightHidOutPtr = AppUtils::loadVectors("mnist_hid_out.bin", vectorInitData);
    const uint32_t outputBiasPtr = AppUtils::loadVectors("mnist_bias.bin", vectorInitData);
    
    // Allocate additional vector arrays
    const uint32_t inputSpikeTimePtr = AppUtils::allocateVectorAndZero(numInput, vectorInitData);

    const uint32_t hiddenIsynPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t hiddenVPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t hiddenRefracTimePtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);

    const uint32_t outputIsynPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
    const uint32_t outputVPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
    const uint32_t outputVSumPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
   
    // Allocate scalar arrays
    const uint32_t timestepPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t inputSpikePtr = AppUtils::allocateScalarAndZero(numInputSpikeWords * 4, scalarInitData);
    const uint32_t hiddenSpikePtr = AppUtils::allocateScalarAndZero(numHiddenSpikeWords * 4, scalarInitData);

    // Load data
    const auto mnistTimes = loadData("mnist_times.bin");
    const auto mnistLabels = loadData("mnist_labels.bin");

    CodeGenerator c;
    {
        VectorRegisterAllocator vectorRegisterAllocator;
        ScalarRegisterAllocator scalarRegisterAllocator;

        // V0 = *timestep
        ALLOCATE_VECTOR(VTime);
        {
            ALLOCATE_SCALAR(STemp);
            c.li(*STemp, timestepPtr);
            c.lh(*STemp, *STemp);
            c.vfill(*VTime, *STemp);
        }

        // Loop over time
        Label timeLoop;
        c.L(timeLoop);
        {
            // ---------------------------------------------------------------
            // Input->Hidden synapses
            // ---------------------------------------------------------------
            // 2^8 = 2 bytes * 128 hidden neurons
            genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator,
                           weightInHidPtr, inputSpikePtr, 
                           hiddenIsynPtr, numInput, numHidden, 8, false);

            // ---------------------------------------------------------------
            // Hidden->Output synapses
            // ---------------------------------------------------------------
            // 2^6 = 2 bytes * 32 output neurons (rounded up)
            genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator, 
                           weightHidOutPtr, hiddenSpikePtr, 
                           outputIsynPtr, numHidden, numOutput, 6, false);

            // ---------------------------------------------------------------
            // Input neurons
            // ---------------------------------------------------------------
            {
                // Register allocation
                ALLOCATE_SCALAR(SSpikeBuffer);
                ALLOCATE_SCALAR(SSpikeTimeBuffer);
                ALLOCATE_SCALAR(SSpikeTimeBufferEnd);

                // Labels
                Label neuronLoop;

                // Get address of spike and spike time buffer
                c.li(*SSpikeBuffer, inputSpikePtr);
                c.li(*SSpikeTimeBuffer, inputSpikeTimePtr);
                c.li(*SSpikeTimeBufferEnd, inputSpikeTimePtr + ((numInput / 32) * 64));

                {
                    // Register allocation
                    ALLOCATE_VECTOR(VSpikeTime);
                    ALLOCATE_SCALAR(SSpikeVec);

                    AppUtils::unrollVectorLoopBody(
                        c, (numInput / 32) * 32, 4, *SSpikeTimeBuffer, *SSpikeTimeBufferEnd,
                        [SSpikeVec, SSpikeBuffer, SSpikeTimeBuffer, VSpikeTime, VTime]
                        (CodeGenerator &c, uint32_t r)
                        {
                            // Load spike times and increment buffer
                            c.vloadv(*VSpikeTime, *SSpikeTimeBuffer, 64 * r);
                            
                            // spike vector = x4 = spike time == t
                            c.vteq(*SSpikeVec, *VTime, *VSpikeTime);

                            // inputSpikeBuffer + scalarOffset = spike vector
                            c.sw(*SSpikeVec, *SSpikeBuffer, 4 * r);
                            },
                            [SSpikeTimeBuffer, SSpikeBuffer]
                            (CodeGenerator &c, uint32_t numUnrolls)
                            {
                                c.addi(*SSpikeTimeBuffer, *SSpikeTimeBuffer, 64 * numUnrolls);
                                c.addi(*SSpikeBuffer, *SSpikeBuffer, 4 * numUnrolls);
                            });
                    // Input neuron tail
                    {
                        // Register allocation
                        ALLOCATE_SCALAR(SMask);
                        
                        // Calculate mask for first iteration
                        c.li(*SMask, (1 << ((numInputSpikeWords * 32) - numInput)) - 1);

                        // Load spike
                        c.vloadv(*VSpikeTime, *SSpikeTimeBuffer);

                        // spike vector = x4 = spike time == t
                        c.vteq(*SSpikeVec, *VTime, *VSpikeTime);
                        c.and_(*SSpikeVec, *SSpikeVec, *SMask);

                        // inputSpikeBuffer + scalarOffset = spike vector
                        c.sw(*SSpikeVec, *SSpikeBuffer);
                    }
                }
            }

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
                 
                AppUtils::unrollVectorLoopBody(
                    c, numHidden, 4, *SVBuffer, *SVBufferEnd,
                    [&scalarRegisterAllocator, &vectorRegisterAllocator,
                    SVBuffer, SISynBuffer, SRefracTimeBuffer, SSpikeBuffer,
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

            // End
            c.ecall();
        }

        LOGI << "Max vector registers used: " << vectorRegisterAllocator.getMaxUsedRegisters();
        LOGI << "Max scalar registers used: " << scalarRegisterAllocator.getMaxUsedRegisters();
    }

    // Create RISC-V core with instruction and scalar data
    const auto code = c.getCode();
    LOGI << code.size() << " instructions";
    RISCV riscV(code, scalarInitData);
    
    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
    // Recording data
    std::vector<uint32_t> inputSpikeRecording;
    std::vector<uint32_t> hiddenSpikeRecording;
    inputSpikeRecording.reserve(79 * numInputSpikeWords);
    hiddenSpikeRecording.reserve(79 * numHiddenSpikeWords);

    // Get pointers to scalar and vector memory
    auto *scalarData = riscV.getScalarDataMemory().getData().data();
    auto *vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData().data();

    // From these, get pointers to data structures
    const uint32_t *inputSpikeWords = reinterpret_cast<const uint32_t*>(scalarData + inputSpikePtr);
    const uint32_t *hiddenSpikeWords = reinterpret_cast<const uint32_t*>(scalarData + hiddenSpikePtr);
    int16_t *outputVSum = vectorData + (outputVSumPtr / 2);

    // Loop through examples
    int numCorrect = 0;
    for(int i = 0; i < 10000; i++) {
        // Show % progress
        const auto iPerc = std::div(i, 100);
        if(iPerc.rem == 0) {
            std:: cout << iPerc.quot << "%" << std::endl;
        }

        // Copy spike times into vector memory
        std::copy_n(mnistTimes.data() + (numInput * i), numInput, vectorData + (inputSpikeTimePtr / 2));

        // Loop through time
        for(uint32_t t = 0; t < 79; t++) {
            // Copy timestep into scalar memory
            std::memcpy(scalarData + timestepPtr, &t, 4);
        
            // Reset PC and run
            riscV.setPC(0);
            if(!riscV.run()) {
                return 1;
            }

            // Record spike words
            //std::copy(inputSpikeWords, inputSpikeWords + numInputSpikeWords,
            //          std::back_inserter(inputSpikeRecording));
            //std::copy(hiddenSpikeWords, hiddenSpikeWords + numHiddenSpikeWords,
            //          std::back_inserter(hiddenSpikeRecording));
        }

        // Determine if output is correct
        const auto classification = std::distance(outputVSum, std::max_element(outputVSum, outputVSum + 10));
        if(classification == mnistLabels[i]) {
            numCorrect++;
        }

        // Zero output V sum
        std::fill_n(outputVSum, 10, 0);
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
    
    
    // Record output spikes
    //std::ofstream inputSpikes("input_spikes.csv");
    //std::ofstream hiddenSpikes("hidden_spikes.csv");
    //for(uint32_t t = 0; t < 79; t++) {
    //    AppUtils::writeSpikes(inputSpikes, inputSpikeRecording.data() + (numInputSpikeWords * t),
    //                          t, numInputSpikeWords);
    //    AppUtils::writeSpikes(hiddenSpikes, hiddenSpikeRecording.data() + (numHiddenSpikeWords * t),
    //                          t, numHiddenSpikeWords);
    //}

    return 0;
}
