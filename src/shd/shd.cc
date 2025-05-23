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

// RISC-V utils include
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


struct StaticPulseTarget
{
    uint32_t weightPtr;
    uint32_t postISynPtr;
    uint32_t numPost;
    uint32_t stride;
    bool debug;
};

void genStaticPulse(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                    ScalarRegisterAllocator &scalarRegisterAllocator,
                    std::variant<uint32_t, ScalarRegisterAllocator::RegisterPtr> preSpikePtr, uint32_t numPre, 
                    const std::vector<StaticPulseTarget> &targets)
{
    // Register allocation
    ALLOCATE_SCALAR(SSpikeBufferEnd);
    ALLOCATE_SCALAR(SWordNStart);
    ALLOCATE_SCALAR(SConst1);
    ALLOCATE_SCALAR(SSpikeWord);

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
    

    // Loop through postsynaptic targets
    std::vector<ScalarRegisterAllocator::RegisterPtr> sISynBufferRegs;
    for(const auto &t : targets) {
        // Allocate scalar registers
        auto bufferStartReg = scalarRegisterAllocator.getRegister("SISynBuffer = X");

        // Load addresses as immediates
        c.li(*bufferStartReg, t.postISynPtr);

        // Add scalar registers to vector
        sISynBufferRegs.emplace_back(bufferStartReg);
    }
    
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

            // Loop through postsynaptic targets
            for(size_t i = 0; i < targets.size(); i++) {
                const auto &t = targets[i];
                const auto &iReg = sISynBufferRegs[i];

                // SWeightBuffer = weightInHidStart + (stride * SN);
                ALLOCATE_SCALAR(SWeightBuffer);
                c.li(*SWeightBuffer, t.weightPtr);
                {
                    ALLOCATE_SCALAR(STemp);
                    c.li(*STemp, t.stride);
                    c.mul(*STemp, *SN, *STemp);
                    c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
                }

                // Reset Isyn pointer
                c.li(*iReg, t.postISynPtr);
                
                // Load weight and Isyn
                if(t.debug) {
                    c.ebreak();
                }

                ALLOCATE_VECTOR(VWeight);
                ALLOCATE_VECTOR(VISyn1);
                ALLOCATE_VECTOR(VISyn2);
                ALLOCATE_VECTOR(VISynNew);

                // Preload first ISyn to avoid stall
                c.vloadv(*VISyn1, *iReg, 0);

                AssemblerUtils::unrollVectorLoopBody(
                    c, scalarRegisterAllocator, t.numPost, 4, *iReg,
                    [&iReg, SWeightBuffer, VWeight, VISyn1, VISyn2, VISynNew]
                    (CodeGenerator &c, uint32_t r, bool even, ScalarRegisterAllocator::RegisterPtr maskReg)
                    {
                        // Load vector of weights
                        c.vloadv(*VWeight, *SWeightBuffer, r * 64);

                        // Load NEXT vector of ISyn to avoid stall
                        // **YUCK** in last iteration, while this may not be accessed, it may be out of bounds                  
                        c.vloadv(even ? *VISyn2 : *VISyn1, *iReg, (r + 1) * 64);
                        
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
                        c.vstore(*VISyn, *iReg, r * 64);
                    },
                    [&iReg, SWeightBuffer]
                    (CodeGenerator &c, uint32_t numUnrolls)
                    {
                        // Increment pointers 
                        c.addi(*iReg, *iReg, 64 * numUnrolls);
                        c.addi(*SWeightBuffer, *SWeightBuffer, 64 * numUnrolls);
                    });
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

int main(int argc, char** argv)
{
    const auto programStartTime = std::chrono::high_resolution_clock::now();

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::info, &consoleAppender);
    
    bool device = false;
    uint32_t numExamples = 2264;

    CLI::App app{"SHD inference"};
    app.add_option("-n,--num-examples", numExamples, "How many examples to simulate");
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");

    CLI11_PARSE(app, argc, argv);
    
    // Allocate memory
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Constants
    constexpr uint32_t numInput = 700;
    constexpr uint32_t numHidden = 256;
    constexpr uint32_t numOutput = 20;
    constexpr uint32_t hiddenVFixedPoint = 9;
    constexpr uint32_t hiddenAFixedPoint = 7;
    constexpr uint32_t outFixedPoint = 11;
    constexpr uint32_t numTimesteps = 1170;
    constexpr uint32_t numInputSpikeWords = ceilDivide(numInput, 32);
    constexpr uint32_t numHiddenSpikeWords = ceilDivide(numHidden, 32);
    constexpr uint32_t numOutputSpikeWords = ceilDivide(numOutput, 32);
    constexpr uint32_t numInputSpikeArrayWords = numInputSpikeWords * numTimesteps;

    // Allocate vector arrays
    const uint32_t seedPtr = AppUtils::allocateVectorSeedAndInit(vectorInitData);
    const uint32_t weightInHidPtr = AppUtils::loadVectors("99-Conn_Pop0_Pop1-g.bin", vectorInitData);
    const uint32_t weightHidOutPtr = AppUtils::loadVectors("99-Conn_Pop1_Pop2-g.bin", vectorInitData);
    const uint32_t weightHidHidPtr = AppUtils::loadVectors("99-Conn_Pop1_Pop1-g.bin", vectorInitData);
    const uint32_t outputBiasPtr = AppUtils::loadVectors("99-Pop2-Bias.bin", vectorInitData);
    
    const uint32_t hiddenIsynPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t hiddenVPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t hiddenAPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t hiddenRefracTimePtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);

    const uint32_t outputIsynPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
    const uint32_t outputVPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
    const uint32_t outputVSumPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
   
    // Allocate scalar arrays
    const uint32_t inputSpikeArrayPtr = AppUtils::allocateScalarAndZero(numInputSpikeArrayWords * 4, scalarInitData);
    const uint32_t hiddenSpikePtr = AppUtils::allocateScalarAndZero(numHiddenSpikeWords * 4, scalarInitData);
    const uint32_t outputVSumScalarPtr = AppUtils::allocateScalarAndZero(numOutput * 2, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Increase scalar memory for buffering
    assert(scalarInitData.size() <= (128 * 1024));
    assert(vectorInitData.size() <= (512 * 1024));

    // Load dataset (this is streamed)
    const auto shdSpikes = AppUtils::loadBinaryData<uint32_t>("shd_spikes.bin");
    const auto shdLabels = AppUtils::loadBinaryData<int16_t>("shd_labels.bin");


    const auto initCode = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // ---------------------------------------------------------------
            // RNG
            // ---------------------------------------------------------------
            {
        	// Register allocation
                ALLOCATE_SCALAR(SSeedBuffer);

                // Get seed pointer
                c.li(*SSeedBuffer, seedPtr);

                // Load RNG seed into dedicated registers
                c.vloadr0(*SSeedBuffer);
                c.vloadr1(*SSeedBuffer, 64);
            }
        });

    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Register allocation
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeEnd);

            // Labels
            Label timeLoop;
            Label spinLoop;
            Label inputSpikeStart;
            Label hiddenSpikeStart;
            Label hiddenNeuronStart;
            Label outputNeuronStart;
            Label outputNeuronEnd;

            // Set timestep range and load ready flag pointer
            c.li(*STime, 0);
            c.li(*STimeEnd, numTimesteps);

            // Loop over time
            c.L(timeLoop);
            {
                // ---------------------------------------------------------------
                // Input->Hidden synapses
                // ---------------------------------------------------------------
                c.L(inputSpikeStart);
                {
                    ALLOCATE_SCALAR(SSpikeArrayBuffer);
            
                    // SSpikeArrayBuffer = inputSpikeArrayPtr + (88 * STime)
                    c.li(*SSpikeArrayBuffer, inputSpikeArrayPtr);
                    {
                        // STemp = number of bytes of full vector
                        ALLOCATE_SCALAR(STmp1);
                        ALLOCATE_SCALAR(STmp2);

                        // **TODO** single cycle DSP multiply!

                        // STime * 64
                        c.slli(*STmp1, *STime, 6);
                    
                        // STime * 16
                        c.slli(*STmp2, *STime, 4);

                        // STime * 80
                        c.add(*STmp1, *STmp1, *STmp2);

                        // STime * 8
                        c.slli(*STmp2, *STime, 3);

                        // STime * 88!
                        c.add(*STmp1, *STmp1, *STmp2);

                        // SSpikeTimeBufferEnd += STime * 88
                        c.add(*SSpikeArrayBuffer, *SSpikeArrayBuffer, *STmp1);
                    }

                    // 2^9 = 2 bytes * 256 hidden neurons
                    genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                   SSpikeArrayBuffer, numInput,
                                   {{weightInHidPtr, hiddenIsynPtr, numHidden, 512, false}});
                }

                // ---------------------------------------------------------------
                // Hidden->Output and Hidden->Hidden synapses
                // ---------------------------------------------------------------
                // 2^6 = 2 bytes * 32 output neurons (rounded up)
                c.L(hiddenSpikeStart);
                genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator, 
                               hiddenSpikePtr, numHidden,
                               {{weightHidOutPtr, outputIsynPtr, numOutput, 64, false},
                                {weightHidHidPtr, hiddenIsynPtr, numHidden, 512, false}});

                // ---------------------------------------------------------------
                // Hidden neurons
                // ---------------------------------------------------------------
                c.L(hiddenNeuronStart);
                {
                    // Register allocation
                    ALLOCATE_SCALAR(SVBuffer);
                    ALLOCATE_SCALAR(SABuffer);
                    ALLOCATE_SCALAR(SISynBuffer);
                    ALLOCATE_SCALAR(SRefracTimeBuffer);
                    ALLOCATE_SCALAR(SSpikeBuffer);
                    ALLOCATE_VECTOR(VAlpha);
                    ALLOCATE_VECTOR(VRho);
                    ALLOCATE_VECTOR(VBeta);
                    ALLOCATE_VECTOR(VVThresh);
                    ALLOCATE_VECTOR(VTauRefrac);
                    ALLOCATE_VECTOR(VDT);
                    ALLOCATE_VECTOR(VZero);
                    ALLOCATE_VECTOR(VOne);
                
                    // Load constants
                    // alpha = e^(-1/20)
                    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / 20.0), 14));
                    c.vlui(*VRho, convertFixedPoint(std::exp(-1.0 / 2000.0), 14));
                    c.vlui(*VBeta, convertFixedPoint(0.0174, hiddenVFixedPoint));
                    c.vlui(*VVThresh, convertFixedPoint(0.6, hiddenVFixedPoint));
                    c.vlui(*VTauRefrac, 4);
                    c.vlui(*VDT, 1);
                    c.vlui(*VZero, 0);
                    c.vlui(*VOne, convertFixedPoint(1.0, hiddenAFixedPoint));

                    // Get address of buffers
                    c.li(*SVBuffer, hiddenVPtr);
                    c.li(*SABuffer, hiddenAPtr);
                    c.li(*SISynBuffer, hiddenIsynPtr);
                    c.li(*SRefracTimeBuffer, hiddenRefracTimePtr);
                    c.li(*SSpikeBuffer, hiddenSpikePtr);

                    // Hidden neuron loop
                    AssemblerUtils::unrollVectorLoopBody(
                        c, scalarRegisterAllocator, numHidden, 4, *SVBuffer,
                        [&scalarRegisterAllocator, &vectorRegisterAllocator,
                        hiddenAFixedPoint,
                         SABuffer, SISynBuffer, SRefracTimeBuffer, SSpikeBuffer, SVBuffer,
                         VAlpha, VBeta, VDT, VOne, VRho, VTauRefrac, VVThresh, VZero]
                        (CodeGenerator &c, uint32_t r, bool, ScalarRegisterAllocator::RegisterPtr maskReg)
                        {
                            assert(!maskReg);

                            // Register allocation
                            ALLOCATE_VECTOR(VV);
                            ALLOCATE_VECTOR(VA);
                            ALLOCATE_VECTOR(VISyn);
                            ALLOCATE_VECTOR(VRefracTime);
                            ALLOCATE_SCALAR(SSpikeOut);
                            ALLOCATE_SCALAR(SRefractory);

                            // Load state variables
                            c.vloadv(*VV, *SVBuffer, r * 64);
                            c.vloadv(*VA, *SABuffer, r * 64);
                            c.vloadv(*VISyn, *SISynBuffer, r * 64);
                            c.vloadv(*VRefracTime, *SRefracTimeBuffer, r * 64);

                            // V *= Alpha
                            c.vmul_rs(14, *VV, *VV, *VAlpha);

                            // V += ISyn
                            c.vadd_s(*VV, *VV, *VISyn);

                            // ISyn = 0
                            c.vlui(*VISyn, 0);

                            // A *= Rho
                            c.vmul_rs(14, *VA, *VA, *VRho);

                            // Refractory = RefracTime > 0.0 (0.0 < VRefracTime)
                            c.vtlt(*SRefractory, *VZero, *VRefracTime);
                            {
                                // VTemp = VRefracTime - VDT
                                ALLOCATE_VECTOR(VTemp);
                                c.vsub(*VTemp, *VRefracTime, *VDT);
                            
                                // VRefracTime = SRefractory ? VTemp : VRefracTime
                                c.vsel(*VRefracTime, *SRefractory, *VTemp);
                            }

                            // SSpikeOut = VV >= (VThres + (Beta * A)) 
                            {
                                ALLOCATE_VECTOR(VTmp);
                                c.vmul_rs(hiddenAFixedPoint, *VTmp, *VA, *VBeta);
                                c.vadd_s(*VTmp, *VTmp, *VVThresh);
                                c.vtge(*SSpikeOut, *VV, *VTmp);
                            }

                            // SpikeOut = SpikeOut && !Refractory
                            {
                                // STemp = !SRefractory
                                ALLOCATE_SCALAR(STemp);
                                c.not_(*STemp, *SRefractory);
                                c.and_(*SSpikeOut, *SSpikeOut, *STemp);
                            }

                            // *SSpikeBuffer = SSpikeOut
                            c.sw(*SSpikeOut, *SSpikeBuffer, r * 4);

                            {
                                ALLOCATE_VECTOR(VTmp1);
                                ALLOCATE_VECTOR(VTmp2);
                                c.vsub_s(*VTmp1, *VV, *VVThresh);
                                c.vadd_s(*VTmp2, *VA, *VOne);

                                // V = SpikeOut ? (V - Vthresh) : V
                                c.vsel(*VV, *SSpikeOut, *VTmp1);

                                // A = SpikeOut ? (A + 1) : A
                                c.vsel(*VA, *SSpikeOut, *VTmp2);
                            }

                            // VRefracTime = SSpikeOut ? VTauRefrac : VRefracTime
                            c.vsel(*VRefracTime, *SSpikeOut, *VTauRefrac);

                            // Store VV, ISyn and refrac time and increment buffers
                            c.vstore(*VV, *SVBuffer, r * 64);
                            c.vstore(*VA, *SABuffer, r * 64);
                            c.vstore(*VISyn, *SISynBuffer, r * 64);
                            c.vstore(*VRefracTime, *SRefracTimeBuffer, r * 64);
                        },
                        [SABuffer, SISynBuffer, SRefracTimeBuffer, SSpikeBuffer, SVBuffer]
                        (CodeGenerator &c, uint32_t numUnrolls)
                        {
                            c.addi(*SVBuffer, *SVBuffer, 64 * numUnrolls);
                            c.addi(*SABuffer, *SABuffer, 64 * numUnrolls);
                            c.addi(*SISynBuffer, *SISynBuffer, 64 * numUnrolls);
                            c.addi(*SRefracTimeBuffer, *SRefracTimeBuffer, 64 * numUnrolls);
                            c.addi(*SSpikeBuffer, *SSpikeBuffer, 4 * numUnrolls);
                        });
                }

                // ---------------------------------------------------------------
                // Output neurons
                // ---------------------------------------------------------------
                c.L(outputNeuronStart);
                {
                    // Register allocation
                    ALLOCATE_SCALAR(SVBuffer);
                    ALLOCATE_SCALAR(SVSumBuffer);
                    ALLOCATE_SCALAR(SISynBuffer);
                    ALLOCATE_SCALAR(SBiasBuffer);
                    ALLOCATE_VECTOR(VAlpha);
                    ALLOCATE_VECTOR(VAvgScale);

                    // Load constants
                    // alpha = e^(-1/20)
                    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / 20.0), 14));
                    c.vlui(*VAvgScale, convertFixedPoint(1.0 / (numTimesteps / 2), outFixedPoint));

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
                        c.li(*SMask, (1 << numOutput) - 1);

                        // Load V, VSum and ISyn
                        c.vloadv(*VV, *SVBuffer);
                        c.vloadv(*VVSum, *SVSumBuffer);
                        c.vloadv(*VISyn, *SISynBuffer);
                        c.vloadv(*VBias, *SBiasBuffer);
 
                        // VV *= VAlpha
                        c.vmul_rs(14, *VVNew, *VV, *VAlpha);

                        // VV += VISyn
                        c.vadd_s(*VVNew, *VVNew, *VISyn);

                        // VV += VBias
                        c.vadd_s(*VVNew, *VVNew, *VBias);

                        // VSum += VV * (1 / T)
                        {
                            // Register allocation
                            ALLOCATE_VECTOR(VTmp);
                            c.vmul_rs(outFixedPoint, *VTmp, *VAvgScale, *VVNew);
                            c.vadd_s(*VVSumNew, *VVSum, *VTmp);
                        }

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
                c.L(outputNeuronEnd);

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

            LOGI << "Input spike start:" << inputSpikeStart.getAddress();
            LOGI << "Hidden spike start:" << hiddenSpikeStart.getAddress();
            LOGI << "Hidden neuron start:" << hiddenNeuronStart.getAddress();
            LOGI << "Output neuron start:" << outputNeuronStart.getAddress();
            LOGI << "Output neuron end:" << outputNeuronEnd.getAddress();
        });

    // Assemble instructions
    LOGI << code.size() << " instructions (" << code.size() * 4 << " bytes)";
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
            
            // Start DMA of data to URAM
            device.getDMAController()->startWrite(0, dmaBuffer, 0, vectorInitData.size() * 2);
    
            // Wait for write to complete
            device.getDMAController()->waitForWriteComplete();
        }

        // Initialisation seeding
        const auto initStartTime = std::chrono::high_resolution_clock::now();
        {
            LOGI << "Copying initialisation instructions (" << initCode.size() * sizeof(uint32_t) << " bytes)";
            device.uploadCode(initCode);

            LOGI << "Running initialisation"; 
            device.setEnabled(true);
            device.waitOnNonZero(readyFlagPtr);
            device.setEnabled(false);
        }
        
        
        // Simulation
        const auto simStartTime = std::chrono::high_resolution_clock::now();
        {
            LOGI << "Copying simulation instructions (" << code.size() * sizeof(uint32_t) << " bytes)";
            device.uploadCode(code);

            // Loop through examples
            int numCorrect = 0;
            std::chrono::duration<double> duration{0};
            const volatile int16_t *outputVSum = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + outputVSumScalarPtr);
            for(int i = 0; i < numExamples; i++) {
                // Show % progress
                const auto iPerc = std::div(i, ceilDivide(numExamples, 100));
                if(iPerc.rem == 0) {
                    std:: cout << iPerc.quot << "%" << std::endl;
                }

                // Copy input spike bits into scalar memory
                device.memcpyDataToDevice(inputSpikeArrayPtr, 
                                          reinterpret_cast<const uint8_t*>(shdSpikes.data() + (numInputSpikeArrayWords * i)),
                                          numInputSpikeArrayWords * 4);

                // Disable core
                device.setEnabled(true);
                const auto startTime = std::chrono::high_resolution_clock::now();
                // Wait until ready flag
                device.waitOnNonZero(readyFlagPtr);
                duration += (std::chrono::high_resolution_clock::now() - startTime);

                // Enable core
                device.setEnabled(false);

                // Determine if output is correct
                const auto classification = std::distance(outputVSum, std::max_element(outputVSum, outputVSum + numOutput));
                if(classification == shdLabels[i]) {
                    numCorrect++;
                }
            }
            const auto simEndTime = std::chrono::high_resolution_clock::now();
            std::cout << numCorrect << " / " << numExamples << " correct (" << 100.0 * (numCorrect / (double)numExamples) << "%)" << std::endl;
            std::cout << "Simulation compute time:" << duration.count() << " seconds" << std::endl;
            std::cout << "Startup time:" << (initStartTime - programStartTime).count() << " seconds" << std::endl;
            std::cout << "Init time:" << (simStartTime - initStartTime).count() << " seconds" << std::endl;
            std::cout << "Simulation time:" << (simEndTime - simStartTime).count() << " seconds" << std::endl;
        }
    }
    else {
        // Build ISE with vector co-processor
        RISCV riscV;
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);

        // Set instructions and vector init data
        riscV.setInstructions(code);
        riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().setData(vectorInitData);

        // Get pointers to scalar memory
        auto *scalarData = riscV.getScalarDataMemory().getData();

        // From this, get pointers to data structures
        const int16_t *outputVSum = reinterpret_cast<const int16_t*>(scalarData + outputVSumScalarPtr);

        // Loop through examples
        int numCorrect = 0;
        for(int i = 0; i < numExamples; i++) {
            // Show % progress
            const auto iPerc = std::div(i, ceilDivide(numExamples, 100));
            if(iPerc.rem == 0) {
                std:: cout << iPerc.quot << "%" << std::endl;
            }

            // Copy input spike bits into scalar memory
            std::copy_n(shdSpikes.data() + (numInputSpikeArrayWords * i), numInputSpikeArrayWords, 
                        reinterpret_cast<uint32_t*>(scalarData + inputSpikeArrayPtr));

            // Reset PC and run
            riscV.setPC(0);
            if(!riscV.run()) {
                return 1;
            }

            // Determine if output is correct
            const auto classification = std::distance(outputVSum, std::max_element(outputVSum, outputVSum + numOutput));
            if(classification == shdLabels[i]) {
                numCorrect++;
            }
        }

        std::cout << numCorrect << " / " << numExamples << " correct (" << 100.0 * (numCorrect / (double)numExamples) << "%)" << std::endl;
        std::cout << "Stats:" << std::endl;
        std::cout << "\t" << riscV.getTotalNumInstructionsExecuted() << " instructions executed" << std::endl;
        std::cout << "\t\t" << riscV.getTotalNumCoprocessorInstructionsExecuted(vectorQuadrant) << " vector instructions executed" << std::endl;
        std::cout << "\t\t" << riscV.getNumJumps() << " jumps" << std::endl;
        std::cout << "\t\t" << riscV.getNumMemory() << " scalar memory" << std::endl;
        std::cout << "\t\t" << riscV.getNumALU() << " scalar ALU" << std::endl;
        std::cout << "\t\t" << riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getNumMemory(riscV.getNumCoprocessorInstructionsExecuted(vectorQuadrant)) << " vector memory" << std::endl;
        std::cout << "\t\t" << riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getNumALU(riscV.getNumCoprocessorInstructionsExecuted(vectorQuadrant)) << " vector ALU" << std::endl;

        std::ofstream heatmapFile("shd_heatmap.txt");
        for(size_t i = 0; i < riscV.getInstructionHeatmap().size(); i++) {
            heatmapFile << (code.at(i) & 0b1111111) << ", " << riscV.getInstructionHeatmap()[i] << std::endl;
        }
    }
   
    
    return 0;
}
