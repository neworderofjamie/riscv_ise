// Standard C++ includes
#include <bitset>
#include <fstream>
#include <iterator>
#include <limits>
#include <memory>
#include <tuple>

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

struct StaticPulseTarget
{
    uint32_t weightPtr;
    uint32_t postISynPtr;
    uint32_t numPost;
    bool debug;
};

void genStaticPulse(CodeGenerator &c, RegisterAllocator<VReg> &vectorRegisterAllocator,
                    RegisterAllocator<Reg> &scalarRegisterAllocator,
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
    std::vector<std::tuple<std::shared_ptr<ScalarRegisterAllocator::Handle>,
                           std::shared_ptr<ScalarRegisterAllocator::Handle>,
                           std::shared_ptr<ScalarRegisterAllocator::Handle>>> targetRegisters;
    for(const auto &t : targets) {
        // Allocate scalar registers
        auto bufferStartReg = scalarRegisterAllocator.getRegister("SISynBuffer = X");
        auto bufferEndReg = scalarRegisterAllocator.getRegister("SISynBufferEnd = X");
        auto numPostStrideReg = scalarRegisterAllocator.getRegister("SNumPostStride = X");

        // Load addresses as immediates
        // **NOTE** end address is only end of loop body not tail
        c.li(*bufferStartReg, t.postISynPtr);
        c.li(*bufferEndReg, t.postISynPtr + (t.numPost / 32) * 64);

        // Load postsynaptic stride a immediate
        c.li(*numPostStrideReg, padSize(t.numPost, 32));

        // Add scalar registers to vector
        targetRegisters.emplace_back(bufferStartReg, bufferEndReg,numPostStrideReg);
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
                const auto &tRegs = targetRegisters[i];

                // Multiply presynaptic neuron index by stride
                ALLOCATE_SCALAR(SWeightBuffer);
                c.li(*SWeightBuffer, t.weightPtr);
                {
                    ALLOCATE_SCALAR(STemp);
                    c.mul(*STemp, *SN, *std::get<2>(tReg));
                    c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
                }

                // Reset Isyn pointer
                c.li(*std::get<0>(tReg), t.postISynPtr);
                
                // Load weight and Isyn
                if(t.debug) {
                    c.ebreak();
                }

                // Loop over postsynaptic neurons
                if(t.numPost > 32) {
                    ALLOCATE_VECTOR(VWeight);
                    ALLOCATE_VECTOR(VISyn1);
                    ALLOCATE_VECTOR(VISyn2);

                    // Preload first ISyn to avoid stall
                    c.vloadv(*VISyn1, *std::get<0>(tReg), 0);

                    AssemblerUtils::unrollVectorLoopBody(
                        c, t.numPost, 4, *std::get<0>(tReg), *std::get<1>(tReg),
                        [&tReg, SWeightBuffer, VWeight, VISyn1, VISyn2]
                        (CodeGenerator &c, uint32_t r)
                        {
                            // Load vector of weights
                            c.vloadv(*VWeight, *SWeightBuffer, r * 64);

                            // Load NEXT vector of ISyn to avoid stall
                            // **YUCK** in last iteration, while this may not be accessed, it may be out of bounds
                            const bool even = ((r % 2) == 0);                            
                            c.vloadv(even ? *VISyn2 : *VISyn1, *std::get<0>(tReg), (r + 1) * 64);
                            
                            // Add weights to ISyn
                            auto VISyn = even ? VISyn1 : VISyn2;
                            c.vadd_s(*VISyn, *VISyn, *VWeight);

                            // Write back ISyn and increment SISynBuffer
                            c.vstore(*VISyn, *std::get<0>(tReg), r * 64);
                        },
                        [&tReg, SWeightBuffer]
                        (CodeGenerator &c, uint32_t numUnrolls)
                        {
                            // Increment pointers 
                            c.addi(*std::get<0>(tReg), *std::get<0>(tReg), 64 * numUnrolls);
                            c.addi(*SWeightBuffer, *SWeightBuffer, 64 * numUnrolls);
                        });
                }
                // Tail if there are non-POT number of postsynaptic neurons
                if((t.numPost % 32) != 0) {
                    ALLOCATE_SCALAR(SMask);
                    ALLOCATE_VECTOR(VWeight);
                    ALLOCATE_VECTOR(VISyn);
                    ALLOCATE_VECTOR(VISynNew);

                    // Calculate mask for final iteration
                    if(t.numPost > 32) {
                        c.li(*SMask, (1 << (padSize(t.numPost, 32) - t.numPost)) - 1);
                    }
                    else {
                        c.li(*SMask, (1 << t.numPost) - 1);
                    }

                    // Load next vector of weights and ISyns
                    c.vloadv(*VWeight, *SWeightBuffer);
                    c.vloadv(*VISyn, *std::get<0>(tReg));

                    // Add weights to ISyn with mask
                    c.vadd_s(*VISynNew, *VISyn, *VWeight);
                    c.vsel(*VISyn, *SMask, *VISynNew);

                    // Write back ISyn
                    c.vstore(*VISyn, *iReg.first);
                }
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


std::vector<uint32_t> generateSimCode(bool simulate, uint32_t numE, uint32_t numI, uint32_t numTimesteps, uint32_t fixedPoint, 
                                      uint32_t numESpikeWords, uint32_t numISpikeWords,
                                      uint32_t eVPtr, uint32_t eRefracTimePtr, uint32_t eSpikePtr,
                                      uint32_t iVPtr, uint32_t iRefracTimePtr, uint32_t iSpikePtr,
                                      uint32_t eeIsynPtr, uint32_t eiIsynPtr, uint32_t ieIsynPtr, uint32_t iiIsynPtr, 
                                      uint32_t readyFlagPtr)
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
                // Inhibitory synapses
                // ---------------------------------------------------------------
                /*genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator, 
                               iSpikePtr, numI,
                               {{weightHidOutPtr, outputIsynPtr, numOutput, false},
                                {weightHidHidPtr, hiddenIsynPtr, numHidden, false}});*/

                // ---------------------------------------------------------------
                // Hidden->Output synapses
                // ---------------------------------------------------------------
                // 2^6 = 2 bytes * 32 output neurons (rounded up)
                /*genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator, 
                               weightHidOutPtr, hiddenSpikePtr, 
                               outputIsynPtr, numHidden, numOutput, false);*/

                
                // ---------------------------------------------------------------
                // E neurons
                // ---------------------------------------------------------------
                {
                    // Register allocation
                    ALLOCATE_SCALAR(SVBuffer);
                    ALLOCATE_SCALAR(SVBufferEnd);
                    ALLOCATE_SCALAR(SEISynBuffer);
                    ALLOCATE_SCALAR(SIISynBuffer)
                    ALLOCATE_SCALAR(SRefracTimeBuffer);
                    ALLOCATE_SCALAR(SSpikeBuffer);
                    ALLOCATE_VECTOR(VAlpha);
                    ALLOCATE_VECTOR(VEBeta);
                    ALLOCATE_VECTOR(VIBeta);
                    ALLOCATE_VECTOR(VEScale);
                    ALLOCATE_VECTOR(VIScale);
                    ALLOCATE_VECTOR(VThresh);
                    ALLOCATE_VECTOR(VVReset);
                    ALLOCATE_VECTOR(VVRest);
                    ALLOCATE_VECTOR(VRMembrane);
                    ALLOCATE_VECTOR(VTauRefrac);
                    ALLOCATE_VECTOR(VDT);
                    ALLOCATE_VECTOR(VZero); 

                    // Load constants
                    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / 20.0), 14));
                    c.vlui(*VEBeta, convertFixedPoint(std::exp(-1.0 / 5.0), 14));
                    c.vlui(*VIBeta, convertFixedPoint(std::exp(-1.0 / 10.0), 14));
                    c.vlui(*VEScale, convertFixedPoint(5.0 * (1.0 - std::exp(-1.0 / 5.0)), fixedPoint));
                    c.vlui(*VIScale, convertFixedPoint(10.0 * (1.0 - std::exp(-1.0 / 10.0)), fixedPoint));
                    c.vlui(*VThresh, convertFixedPoint(-50.0, fixedPoint));
                    c.vlui(*VVReset, convertFixedPoint(-60.0, fixedPoint));
                    c.vlui(*VVRest, convertFixedPoint(-49.0, fixedPoint));
                    c.vlui(*VRMembrane, convertFixedPoint(20.0 / 1.0, fixedPoint));
                    c.vlui(*VTauRefrac, 5);
                    c.vlui(*VDT, 1);
                    c.vlui(*VZero, 0);

                    // Get address of buffers
                    c.li(*SVBuffer, eVPtr);
                    c.li(*SVBufferEnd, eVPtr + (numESpikeWords * 64));
                    c.li(*SEISynBuffer, eeIsynPtr);
                    c.li(*SIISynBuffer, ieIsynPtr);
                    c.li(*SRefracTimeBuffer, eRefracTimePtr);
                    c.li(*SSpikeBuffer, eSpikePtr);
                 
                    AssemblerUtils::unrollVectorLoopBody(
                        c, numE, 4, *SVBuffer, *SVBufferEnd,
                        [&scalarRegisterAllocator, &vectorRegisterAllocator,
                         fixedPoint, 
                         SVBuffer, SEISynBuffer, SIISynBuffer, SRefracTimeBuffer, SSpikeBuffer,
                         VAlpha, VEBeta, VIBeta, VEScale, VIScale, VDT, VRMembrane, VTauRefrac, VThresh, VVReset, VVRest, VZero]
                        (CodeGenerator &c, uint32_t r)
                        {
                            // Register allocation
                            ALLOCATE_VECTOR(VV);
                            ALLOCATE_VECTOR(VEISyn);
                            ALLOCATE_VECTOR(VIISyn);
                            ALLOCATE_VECTOR(VInSyn);
                            ALLOCATE_VECTOR(VRefracTime);
                            ALLOCATE_SCALAR(SSpikeOut);
                            ALLOCATE_SCALAR(SRefractory);
                            ALLOCATE_SCALAR(SNonRefractory);

                            // Load voltage and isyn
                            c.vloadv(*VV, *SVBuffer, 64 * r);
                            c.vloadv(*VEISyn, *SEISynBuffer, 64 * r);
                            c.vloadv(*VIISyn, *SIISynBuffer, 64 * r);
                            c.vloadv(*VRefracTime, *SRefracTimeBuffer, 64 * r);
                            
                            // Excitatory
                            {
                                // Scale VEISyn 
                                c.vmul(fixedPoint, *VInSyn, *VEISyn, *VEScale);

                                // Decay VEIsyn
                                c.vmul(14, *VEISyn, *VEISyn, *VEBeta);
                            }

                            // Inhibitory
                            {
                                ALLOCATE_VECTOR(VTmp);

                                // Scale VIISyn 
                                c.vmul(fixedPoint, *VTmp, *VEISyn, *VEScale);
                                c.vadd(*VInSyn, *VInSyn, *VTmp);

                                // Decay VIISyn
                                c.vmul(14, *VIISyn, *VIISyn, *VIBeta);

                            }
                            
                            // SRefractory = VRefracTime > 0.0 (0.0 < VRefracTime)
                            c.vtlt(*SRefractory, *VZero, *VRefracTime);
                            {
                                ALLOCATE_VECTOR(VRefracTimeTemp);
                                ALLOCATE_VECTOR(VAlphaTemp);
                                ALLOCATE_VECTOR(VVTemp);

                                // VRefractTimeTemp = VRefracTime - VDT
                                c.vsub(*VRefracTimeTemp, *VRefracTime, *VDT);

                                // VAlphaTemp = (VInSyn * VRMembrane) + VVRest
                                c.vmul(fixedPoint, *VAlphaTemp, *VInSyn, *VRMembrane);
                                c.vadd(*VAlphaTemp, *VAlphaTemp, *VVRest);

                                // VVTemp = VAlpha * (VAlphaTemp - VV)
                                c.vsub(*VVTemp, *VAlphaTemp, *VV);
                                c.vmul(fixedPoint, *VVTemp, *VVTemp, *VAlpha);

                                // VVTemp = VAlphaTemp - VVTemp
                                c.vsub(*VVTemp, *VAlphaTemp, *VVTemp);

                                // SNonRefactory = !SRefractory
                                c.not_(*SNonRefractory, *SRefractory);
                                
                                // VRefracTime = SRefractory ? VRefracTimeTemp : VRefracTime
                                c.vsel(*VRefracTime, *SRefractory, *VRefracTimeTemp);

                                // VV = SNonRefractory ? VVTemp : VV
                                c.vsel(*VV, *SNonRefractory, *VVTemp);
                            }

                            // SSpikeOut = VV >= VThresh && !SRefractory
                            c.vtge(*SSpikeOut, *VV, *VThresh);
                            c.and_(*SSpikeOut, *SSpikeOut, *SNonRefractory);
                            
                            // *SSpikeBuffer = SSpikeOut
                            c.sw(*SSpikeOut, *SSpikeBuffer, 4 * r);

                            // VV = SSpikeOut ? VVReset : VV
                            c.vsel(*VV, *SSpikeOut, *VVReset);

                            // VRefracTime = SSpikeOut ? VTauRefrac : VRefracTime
                            c.vsel(*VRefracTime, *SSpikeOut, *VTauRefrac);

                            // Store VV, ISyn and refrac time and increment buffers
                            c.vstore(*VV, *SVBuffer, 64 * r);
                            c.vstore(*VIISyn, *SIISynBuffer, 64 * r);
                            c.vstore(*VEISyn, *SEISynBuffer, 64 * r);
                            c.vstore(*VRefracTime, *SRefracTimeBuffer, 64 * r);
                        },
                        [SEISynBuffer, SIISynBuffer, SRefracTimeBuffer, SSpikeBuffer, SVBuffer]
                        (CodeGenerator &c, uint32_t numUnrolls)
                        {
                            c.addi(*SVBuffer, *SVBuffer, 64 * numUnrolls);
                            c.addi(*SEISynBuffer, *SEISynBuffer, 64 * numUnrolls);
                            c.addi(*SIISynBuffer, *SIISynBuffer, 64 * numUnrolls);
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
    constexpr uint32_t numInputSpikeWords = ceilDivide(numInput, 32);
    constexpr uint32_t numHiddenSpikeWords = ceilDivide(numHidden, 32);
    constexpr uint32_t numOutputSpikeWords = ceilDivide(numOutput, 32);
    constexpr uint32_t numInputSpikeArrayWords = numInputSpikeWords * numTimesteps;

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
    const auto simCode = generateSimCode(simulate, numInput, numHidden, numOutput, numTimesteps,
                                         hiddenFixedPoint, outFixedPoint, numInputSpikeWords,
                                         numHiddenSpikeWords,  weightInHidPtr, weightHidOutPtr, 
                                         outputBiasPtr, hiddenIsynPtr, hiddenVPtr,
                                         hiddenRefracTimePtr, outputIsynPtr, outputVPtr, outputVSumPtr,
                                         inputSpikeArrayPtr, hiddenSpikePtr, outputVSumScalarPtr, readyFlagPtr);
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

        // Recording data
        //std::vector<uint32_t> inputSpikeRecording;
        //std::vector<uint32_t> hiddenSpikeRecording;
        //inputSpikeRecording.reserve(79 * numInputSpikeWords);
        ///hiddenSpikeRecording.reserve(79 * numHiddenSpikeWords);

        // From these, get pointers to data structures
        // 
        //const uint32_t *inputSpikeWords = reinterpret_cast<const uint32_t*>(scalarData + inputSpikePtr);
        //const uint32_t *hiddenSpikeWords = reinterpret_cast<const uint32_t*>(scalarData + hiddenSpikePtr);
        auto *scalarData = riscV.getScalarDataMemory().getData().data();
        const int16_t *outputVSum = reinterpret_cast<const int16_t*>(scalarData + outputVSumScalarPtr);

        // Loop through examples
        int numCorrect = 0;
        for(size_t i = 0; i < 10000; i++) {
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
    
    
        // Record output spikes
        //std::ofstream inputSpikes("input_spikes.csv");
        //std::ofstream hiddenSpikes("hidden_spikes.csv");
        //for(uint32_t t = 0; t < 79; t++) {
        //    AppUtils::writeSpikes(inputSpikes, inputSpikeRecording.data() + (numInputSpikeWords * t),
        //                          t, numInputSpikeWords);
        //    AppUtils::writeSpikes(hiddenSpikes, hiddenSpikeRecording.data() + (numHiddenSpikeWords * t),
        //                          t, numHiddenSpikeWords);
        //}
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
            for(size_t i = 0; i < 10000; i++) {
                // Show % progress
                const auto iPerc = std::div(i, 100);
                if(iPerc.rem == 0) {
                    std:: cout << iPerc.quot << "%" << std::endl;
                }

                // Copy input spike bits into scalar memory
                device.memcpyDataToDevice(inputSpikeArrayPtr, 
                                          reinterpret_cast<const uint8_t*>(mnistSpikes.data() + (numInputSpikeArrayWords * i)),
                                          numInputSpikeArrayWords * 4);

                // Put core into running state
                LOGI << "Enabling";
                device.setEnabled(true);

                // Wait until ready flag
                device.waitOnNonZero(readyFlagPtr);

                // Reset core
                LOGI << "Disabling";
                device.setEnabled(false);

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
