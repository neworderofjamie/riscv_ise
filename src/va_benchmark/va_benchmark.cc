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
#include "common/CLI11.hpp"
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

#define RECORD_SPIKES

struct StaticPulseTarget
{
    uint32_t postIndPtr;
    VReg weight;
    uint32_t maxRowLength;
    uint32_t laneLocalImm;
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
    ALLOCATE_VECTOR(VZero);

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
    std::vector<std::pair<std::shared_ptr<ScalarRegisterAllocator::Handle>,
                          std::shared_ptr<ScalarRegisterAllocator::Handle>>> targetRegisters;
    for(const auto &t : targets) {
        // Allocate scalar registers
        auto bufferStartReg = scalarRegisterAllocator.getRegister("SIndBuffer = X");

        auto numPostStrideReg = scalarRegisterAllocator.getRegister("SNumPostStride = X");

        // Load addresses as immediates
        c.li(*bufferStartReg, t.postIndPtr);

        // Load postsynaptic stride a immediate
        c.li(*numPostStrideReg, padSize(t.maxRowLength, 32));

        // Add scalar registers to vector
        targetRegisters.emplace_back(bufferStartReg, numPostStrideReg);
    }
    
    // Load some useful constants
    c.li(*SConst1, 1);

    // SWordNStart = 31
    c.li(*SWordNStart, 31);

    c.vlui(*VZero, 0);
        
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
                const auto &tReg = targetRegisters[i];

                // Multiply presynaptic neuron index by stride
                ALLOCATE_SCALAR(SPostIndBuffer);
                c.li(*SPostIndBuffer, t.postIndPtr);
                {
                    ALLOCATE_SCALAR(STemp);
                    c.mul(*STemp, *SN, *tReg.second);
                    c.add(*SPostIndBuffer, *SPostIndBuffer, *STemp);
                }
                
                // Load weight and Isyn
                if(t.debug) {
                    c.ebreak();
                }

                // Loop over postsynaptic neurons
                auto VWeight = t.weight;
                ALLOCATE_SCALAR(SMask);
                ALLOCATE_VECTOR(VPostInd1);
                ALLOCATE_VECTOR(VPostInd2);
                ALLOCATE_VECTOR(VAccum1);
                ALLOCATE_VECTOR(VAccum2);
                ALLOCATE_VECTOR(VAccumNew);
                
                // Preload first index and accumulator
                c.vloadv(*VPostInd1, *tReg.first);
                c.nop();
                c.vloadl(*VAccum1, *VPostInd1, t.laneLocalImm);

                const uint32_t laneLocalImm = t.laneLocalImm;
                AssemblerUtils::unrollVectorLoopBody(
                    c, scalarRegisterAllocator, t.maxRowLength, 4, *tReg.first,
                    [&tReg, laneLocalImm, SMask, SPostIndBuffer, VPostInd1, VPostInd2, VAccum1, VAccum2, VAccumNew, VWeight, VZero]
                    (CodeGenerator &c, uint32_t r, uint32_t i, ScalarRegisterAllocator::RegisterPtr maskReg)
                    {
                        assert(!maskReg);

                        // Load vector of postsynaptic indices for next iteration
                        // **YUCK** in last iteration, while this may not be accessed, it may be out of bounds
                        const bool even = ((i % 2) == 0);                            
                        c.vloadv(even ? *VPostInd2 : *VPostInd1, 
                                 *tReg.first, (r + 1) * 64);
                            
                        // Test whether any of the postsynaptic indices from previous iteration are >= 0
                        c.vtge(*SMask, even ? *VPostInd1 : *VPostInd2, *VZero);

                        // Using postsynaptic indices, load accumulator for next iteration
                        c.vloadl(even ? *VAccum2 : *VAccum1, even ? *VPostInd2 : *VPostInd1,
                                 laneLocalImm);

                        // Add weights to accumulator loaded in previous iteration
                        auto VAccum = even ? VAccum1 : VAccum2;
                        c.vadd_s(*VAccumNew, *VAccum, VWeight);
                        c.vsel(*VAccum, *SMask, *VAccumNew);

                        // Write back accumulator
                        c.vstorel(*VAccum, even ? *VPostInd1 : *VPostInd2);
                    },
                    [&tReg]
                    (CodeGenerator &c, uint32_t numUnrolls)
                    {
                        // Increment pointers 
                        c.addi(*tReg.first, *tReg.first, 64 * numUnrolls);
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



int main(int argc, char** argv)
{
    const auto programStartTime = std::chrono::high_resolution_clock::now();

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    bool device = false;
    uint32_t numTimesteps = 1000;

    CLI::App app{"VA benchmark"};
    app.add_option("-n,--num-timesteps", numTimesteps, "How many timesteps to simulate");
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");

    CLI11_PARSE(app, argc, argv);

    // Allocate memory
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Constants
    constexpr uint32_t fixedPoint = 8;
    constexpr uint32_t numExc = 410;
    constexpr uint32_t numInh = 102;
    constexpr uint32_t neuronFixedPoint = 5;
    constexpr uint32_t numExcWords = ceilDivide(numExc, 32);
    constexpr uint32_t numInhWords = ceilDivide(numInh, 32);
    constexpr uint32_t numExcIncomingVectors = 7;
    constexpr uint32_t numInhIncomingVectors = 3;
    const uint32_t numExcSpikeRecordingWords = numExcWords * numTimesteps;
    const uint32_t numInhSpikeRecordingWords = numInhWords * numTimesteps;

    // Layout lane local memory
    constexpr uint32_t eeLLAddr = 0;
    constexpr uint32_t eiLLAddr = eeLLAddr + (numExcWords * 2);
    constexpr uint32_t iiLLAddr = eiLLAddr + (numInhWords * 2);
    constexpr uint32_t ieLLAddr = iiLLAddr + (numInhWords * 2);

    // Allocate vector arrays
    // **NOTE** these are adjacent so data can be block-copied from scalar memory
    const uint32_t seedPtr = AppUtils::allocateVectorAndZero(32 * 2, vectorInitData);
    const uint32_t eeIndPtr = AppUtils::allocateVectorAndZero(numExc * numExcIncomingVectors * 32, vectorInitData);
    const uint32_t eiIndPtr = AppUtils::allocateVectorAndZero(numExc * numInhIncomingVectors * 32, vectorInitData);
    const uint32_t iiIndPtr = AppUtils::allocateVectorAndZero(numInh * numInhIncomingVectors * 32, vectorInitData);
    const uint32_t ieIndPtr = AppUtils::allocateVectorAndZero(numInh * numExcIncomingVectors * 32, vectorInitData);

    const uint32_t excVPtr = AppUtils::allocateVectorAndZero(numExc, vectorInitData);
    const uint32_t excRefracTimePtr = AppUtils::allocateVectorAndZero(numExc, vectorInitData);

    const uint32_t inhVPtr = AppUtils::allocateVectorAndZero(numInh, vectorInitData);
    const uint32_t inhRefracTimePtr = AppUtils::allocateVectorAndZero(numInh, vectorInitData);
    
    // Allocate scalar arrays
    const uint32_t excSpikeArrayPtr = AppUtils::allocateScalarAndZero(numExcWords * 4, scalarInitData);
    const uint32_t inhSpikePtr = AppUtils::allocateScalarAndZero(numInhWords * 4, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

#ifdef RECORD_SPIKES
     const uint32_t excSpikeRecordingPtr = AppUtils::allocateScalarAndZero(numExcSpikeRecordingWords * 4, scalarInitData);
#endif
    //const uint32_t exc
    // Increase scalar memory size
    scalarInitData.resize(64 * 1024, 0);

    // Load connectivity data
    const auto eeInd = AppUtils::loadBinaryData<uint8_t>("va_benchmark_ee.bin");
    const auto eiInd = AppUtils::loadBinaryData<uint8_t>("va_benchmark_ei.bin");
    const auto iiInd = AppUtils::loadBinaryData<uint8_t>("va_benchmark_ii.bin");
    const auto ieInd = AppUtils::loadBinaryData<uint8_t>("va_benchmark_ie.bin");

    // Load seed
    const auto seed = AppUtils::getSeedData();

    // Check first two are correctly padded
    assert(eeInd.size() == numExc * numExcIncomingVectors * 64);
    assert(eiInd.size() == numExc * numInhIncomingVectors * 64);
    assert(iiInd.size() == numInh * numInhIncomingVectors * 64);
    assert(ieInd.size() == numInh * numExcIncomingVectors * 64);
    
    // Concatenate into single vector
    std::vector<uint8_t> copyData;
    copyData.reserve(seed.size() + eeInd.size() + eiInd.size() + iiInd.size() + ieInd.size());
    std::copy(seed.cbegin(), seed.cend(), std::back_inserter(copyData));
    std::copy(eeInd.cbegin(), eeInd.cend(), std::back_inserter(copyData));
    std::copy(eiInd.cbegin(), eiInd.cend(), std::back_inserter(copyData));
    std::copy(iiInd.cbegin(), iiInd.cend(), std::back_inserter(copyData));
    std::copy(ieInd.cbegin(), ieInd.cend(), std::back_inserter(copyData));

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

            // ---------------------------------------------------------------
            // Exc neurons
            // ---------------------------------------------------------------
            {
                // Register allocation
                ALLOCATE_SCALAR(SVBuffer);
                ALLOCATE_SCALAR(SRefracTimeBuffer);
                ALLOCATE_VECTOR(VZero);
                ALLOCATE_VECTOR(VScale);
                ALLOCATE_VECTOR(VRandom);
                ALLOCATE_VECTOR(VSynLLOffset);
                ALLOCATE_VECTOR(VNumUnrollBytes);
                
                // Load constants
                c.vlui(*VZero, 0);
                c.vlui(*VScale, convertFixedPoint(10.0, fixedPoint));
                c.vlui(*VSynLLOffset, 0);
                c.vlui(*VNumUnrollBytes, 2 * std::min(numExcWords, 4u));

                // Get address of buffers
                c.li(*SVBuffer, excVPtr);
                c.li(*SRefracTimeBuffer, excRefracTimePtr);

                // Excitatory neuron loop
                // **NOTE** all these allocations are padded so we overfill to simplify code
                AssemblerUtils::unrollVectorLoopBody(
                    c, scalarRegisterAllocator, numExcWords * 32, 4, *SVBuffer,
                    [&scalarRegisterAllocator, &vectorRegisterAllocator,
                    fixedPoint, eeLLAddr, eiLLAddr,
                    SRefracTimeBuffer, SVBuffer, VRandom, VScale, VSynLLOffset, VZero]
                    (CodeGenerator &c, uint32_t r, uint32_t, ScalarRegisterAllocator::RegisterPtr)
                    {
                        // Generate initial mebrane voltages from U(0,Scale) 
                        c.vrng(*VRandom);
                        c.vmul(15, *VRandom, *VRandom, *VScale);

                        c.vstorel(*VZero, *VSynLLOffset, eeLLAddr + (r * 2));
                        c.vstorel(*VZero, *VSynLLOffset, eiLLAddr + (r * 2));
                        c.vstore(*VRandom, *SVBuffer, r * 64);
                        c.vstore(*VZero, *SRefracTimeBuffer, r * 64);
                    },
                    [SRefracTimeBuffer, SVBuffer, VNumUnrollBytes, VSynLLOffset]
                    (CodeGenerator &c, uint32_t numUnrolls)
                    {
                        c.addi(*SVBuffer, *SVBuffer, 64 * numUnrolls);
                        c.addi(*SRefracTimeBuffer, *SRefracTimeBuffer, 64 * numUnrolls);
                        c.vadd(*VSynLLOffset, *VSynLLOffset, *VNumUnrollBytes);
                    });
            }
        });

    // Generate sim code
    const auto simCode = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Register allocation
            ALLOCATE_SCALAR(STime);
            ALLOCATE_SCALAR(STimeEnd);
#ifdef RECORD_SPIKES
            ALLOCATE_SCALAR(SExcSpikeRecordingBuffer);
#endif
            // Labels
            Label timeLoop;
            Label spinLoop;

            // Set timestep range
            c.li(*STime, 0);
            c.li(*STimeEnd, numTimesteps);
#ifdef RECORD_SPIKES
            c.li(*SExcSpikeRecordingBuffer, excSpikeRecordingPtr);
#endif
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
                    ALLOCATE_SCALAR(SRefracTimeBuffer);
                    ALLOCATE_SCALAR(SSpikeBuffer);
                    ALLOCATE_VECTOR(VAlpha);
                    ALLOCATE_VECTOR(VEBeta);
                    ALLOCATE_VECTOR(VIBeta);
                    ALLOCATE_VECTOR(VEScale);
                    ALLOCATE_VECTOR(VIScale);
                    ALLOCATE_VECTOR(VThresh);
                    ALLOCATE_VECTOR(VIOffset);
                    ALLOCATE_VECTOR(VRMembrane);
                    ALLOCATE_VECTOR(VTauRefrac);
                    ALLOCATE_VECTOR(VDT);
                    ALLOCATE_VECTOR(VZero); 
                    ALLOCATE_VECTOR(VSynLLOffset);
                    ALLOCATE_VECTOR(VNumUnrollBytes);

                    // Load constants
                    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / 20.0), 14));
                    c.vlui(*VEBeta, convertFixedPoint(std::exp(-1.0 / 5.0), 14));
                    c.vlui(*VIBeta, convertFixedPoint(std::exp(-1.0 / 10.0), 14));
                    c.vlui(*VEScale, convertFixedPoint(5.0 * (1.0 - std::exp(-1.0 / 5.0)), fixedPoint));
                    c.vlui(*VIScale, convertFixedPoint(10.0 * (1.0 - std::exp(-1.0 / 10.0)), fixedPoint));
                    c.vlui(*VThresh, convertFixedPoint(10.0, fixedPoint));
                    c.vlui(*VIOffset, convertFixedPoint(5.5, fixedPoint));
                    c.vlui(*VRMembrane, convertFixedPoint(20.0 / 1.0, fixedPoint));
                    c.vlui(*VTauRefrac, 5);
                    c.vlui(*VDT, 1);
                    c.vlui(*VZero, 0);
                    c.vlui(*VSynLLOffset, 0);
                    c.vlui(*VNumUnrollBytes, 2 * std::min(numExcWords, 4u));

                    // Get address of buffers
                    c.li(*SVBuffer, excVPtr);
                    c.li(*SRefracTimeBuffer, excRefracTimePtr);
                    c.li(*SSpikeBuffer, excSpikeArrayPtr);
                 
                    AssemblerUtils::unrollVectorLoopBody(
                        c, scalarRegisterAllocator, numExc, 4, *SVBuffer,
                        [&scalarRegisterAllocator, &vectorRegisterAllocator,
                         fixedPoint, eeLLAddr, eiLLAddr,
                         SVBuffer, SRefracTimeBuffer, SSpikeBuffer,
 #ifdef RECORD_SPIKES
                         SExcSpikeRecordingBuffer,
 #endif
                         VAlpha, VEBeta, VIBeta, VEScale, VIScale, VDT, VRMembrane, VSynLLOffset, VTauRefrac, VThresh, VIOffset, VZero]
                        (CodeGenerator &c, uint32_t r, uint32_t, ScalarRegisterAllocator::RegisterPtr maskReg)
                        {
                            // Register allocation
                            ALLOCATE_VECTOR(VV);
                            ALLOCATE_VECTOR(VESyn);
                            ALLOCATE_VECTOR(VISyn);
                            ALLOCATE_VECTOR(VInSyn);
                            ALLOCATE_VECTOR(VRefracTime);
                            ALLOCATE_SCALAR(SSpikeOut);
                            ALLOCATE_SCALAR(SRefractory);
                            ALLOCATE_SCALAR(SNonRefractory);

                            // Load voltage and isyn
                            c.vloadv(*VV, *SVBuffer, 64 * r);
                            c.vloadl(*VESyn, *VSynLLOffset, eeLLAddr + (r * 2));
                            c.vloadl(*VISyn, *VSynLLOffset, eiLLAddr + (r * 2));
                            c.vloadv(*VRefracTime, *SRefracTimeBuffer, 64 * r);
                            
                            // Excitatory
                            {
                                // Scale VEISyn 
                                c.vmul_rs(fixedPoint, *VInSyn, *VESyn, *VEScale);

                                // Decay VEIsyn
                                c.vmul_rs(14, *VESyn, *VESyn, *VEBeta);
                            }

                            // Inhibitory
                            {
                                ALLOCATE_VECTOR(VTmp);

                                // Scale VIISyn 
                                c.vmul_rs(fixedPoint, *VTmp, *VISyn, *VEScale);
                                c.vadd_s(*VInSyn, *VInSyn, *VTmp);

                                // Decay VIISyn
                                c.vmul_rs(14, *VISyn, *VISyn, *VIBeta);
                            }
                            
                            // SRefractory = VRefracTime > 0.0 (0.0 < VRefracTime)
                            c.vtlt(*SRefractory, *VZero, *VRefracTime);
                            {
                                ALLOCATE_VECTOR(VRefracTimeTemp);
                                ALLOCATE_VECTOR(VAlphaTemp);
                                ALLOCATE_VECTOR(VVTemp);

                                // VRefractTimeTemp = VRefracTime - VDT
                                c.vsub_s(*VRefracTimeTemp, *VRefracTime, *VDT);

                                // VAlphaTemp = (VInSyn + VIoffset) * VRMembrane
                                c.vadd_s(*VAlphaTemp, *VInSyn, *VIOffset);
                                c.vmul_rs(fixedPoint, *VAlphaTemp, *VAlphaTemp, *VRMembrane);

                                // VVTemp = VAlpha * (VAlphaTemp - VV)
                                c.vsub(*VVTemp, *VAlphaTemp, *VV);
                                c.vmul_rs(fixedPoint, *VVTemp, *VVTemp, *VAlpha);

                                // VVTemp = VAlphaTemp - VVTemp
                                c.vsub_s(*VVTemp, *VAlphaTemp, *VVTemp);

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
                            
                            // If we have a mask, and spikes with it
                            if(maskReg) {
                                c.and_(*SSpikeOut, *SSpikeOut, *maskReg);
                            }
                            
                            // *SSpikeBuffer = SSpikeOut
                            c.sw(*SSpikeOut, *SSpikeBuffer, 4 * r);
#ifdef RECORD_SPIKES
                            c.sw(*SSpikeOut, *SExcSpikeRecordingBuffer);
                            c.addi(*SExcSpikeRecordingBuffer, *SExcSpikeRecordingBuffer, 4);
#endif
                            // VV = SSpikeOut ? VZero : VV
                            c.vsel(*VV, *SSpikeOut, *VZero);

                            // VRefracTime = SSpikeOut ? VTauRefrac : VRefracTime
                            c.vsel(*VRefracTime, *SSpikeOut, *VTauRefrac);

                            // Store VV and refrac time and increment buffers
                            c.vstore(*VV, *SVBuffer, 64 * r);
                            c.vstore(*VRefracTime, *SRefracTimeBuffer, 64 * r);

                            // Store VESyn and VIsyn back to lane-local memory
                            c.vstorel(*VESyn, *VSynLLOffset, eeLLAddr + (r * 2));
                            c.vstorel(*VISyn, *VSynLLOffset, eiLLAddr + (r * 2));
                        },
                        [SRefracTimeBuffer, SSpikeBuffer, SVBuffer, VNumUnrollBytes, VSynLLOffset]
                        (CodeGenerator &c, uint32_t numUnrolls)
                        {
                            c.addi(*SVBuffer, *SVBuffer, 64 * numUnrolls);
                            c.addi(*SRefracTimeBuffer, *SRefracTimeBuffer, 64 * numUnrolls);
                            c.addi(*SSpikeBuffer, *SSpikeBuffer, 4 * numUnrolls); 
                            c.vadd(*VSynLLOffset, *VSynLLOffset, *VNumUnrollBytes);
                        });
                }

                c.addi(*STime, *STime, 1);
                c.bne(*STime, *STimeEnd, timeLoop);
            }
        });
    LOGI << simCode.size() << " simulation instructions";
    LOGI << scalarInitData.size() << " bytes of scalar memory required";
    LOGI << vectorInitData.size() * 2 << " bytes of vector memory required (" << ceilDivide(vectorInitData.size() / 32, 4096) << " URAM cascade)";

    // Generate initialisation code to copy blocks of data from scalar to vector memory
    const uint32_t copyStartVectorPtr = 0;
    const uint32_t copyNumVectorsPtr = 4;
    const uint32_t copyReadyFlagPtr = 8;
    const uint32_t copyScalarScratchPtr = 12;
    const auto copyCode = AssemblerUtils::generateInitCode(!device, copyStartVectorPtr, copyNumVectorsPtr, 
                                                           copyReadyFlagPtr, copyScalarScratchPtr);
    
    if(device) {
        LOGI << "Creating device";
        Device device;

        // Put core into reset state
        LOGI << "Resetting";
        device.setEnabled(false);
        
        // Copying
        const auto copyStartTime = std::chrono::high_resolution_clock::now();
        {
            LOGI << "Copying copy instructions (" << copyCode.size() * sizeof(uint32_t) << " bytes)";
            device.uploadCode(copyCode);

            // Run kernels to copy initData into vector memory
            device.runInit(copyData, copyStartVectorPtr, copyNumVectorsPtr, copyScalarScratchPtr, seedPtr, copyReadyFlagPtr);
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
            LOGI << "Copying simulation instructions (" << simCode.size() * sizeof(uint32_t) << " bytes)";
            device.uploadCode(simCode);
            
            // Put core into running state
            LOGI << "Enabling";
            device.setEnabled(true);

            // Wait until ready flag
            device.waitOnNonZero(readyFlagPtr);

            // Reset core
            LOGI << "Disabling";
            device.setEnabled(false);
        }

        const auto simEndTime = std::chrono::high_resolution_clock::now();
        std::cout << "Startup time:" << (copyStartTime - programStartTime).count() << " seconds" << std::endl;
        std::cout << "Copy time:" << (initStartTime - copyStartTime).count() << " seconds" << std::endl;
        std::cout << "Init time:" << (simStartTime - initStartTime).count() << " seconds" << std::endl;
        std::cout << "Simulation time:" << (simEndTime - simStartTime).count() << " seconds" << std::endl;
    }
    else {
        // Create RISC-V core with instruction and scalar data
        RISCV riscV(copyCode, scalarInitData);
    
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
        
        // Run kernels to copy initData into vector memory
        if(!riscV.runInit(copyData, copyStartVectorPtr, copyNumVectorsPtr, copyScalarScratchPtr, seedPtr)) {
            return 1;
        }

        // Reset stats for simulations
        riscV.resetStats();

        // Load init program
        riscV.setInstructions(initCode);

        // Run RISC-V to initialize
        riscV.setPC(0);
        if(!riscV.run()) {
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
        //auto *scalarData = riscV.getScalarDataMemory().getData().data();
        //const int16_t *outputVSum = reinterpret_cast<const int16_t*>(scalarData + outputVSumScalarPtr);


        // Reset PC and run
        riscV.setPC(0);
        if(!riscV.run()) {
            return 1;
        }

        std::cout << "Stats:" << std::endl;
        std::cout << "\t" << riscV.getTotalNumInstructionsExecuted() << " instructions executed" << std::endl;
        std::cout << "\t\t" << riscV.getTotalNumCoprocessorInstructionsExecuted(vectorQuadrant) << " vector instructions executed" << std::endl;
        std::cout << "\t\t" << riscV.getNumJumps() << " jumps" << std::endl;
        std::cout << "\t\t" << riscV.getNumMemory() << " scalar memory" << std::endl;
        std::cout << "\t\t" << riscV.getNumALU() << " scalar ALU" << std::endl;
        std::cout << "\t\t" << riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getNumMemory(riscV.getNumCoprocessorInstructionsExecuted(vectorQuadrant)) << " vector memory" << std::endl;
        std::cout << "\t\t" << riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getNumALU(riscV.getNumCoprocessorInstructionsExecuted(vectorQuadrant)) << " vector ALU" << std::endl;
    
#ifdef RECORD_SPIKES
        auto *scalarData = riscV.getScalarDataMemory().getData().data();
        const uint32_t *excSpikeRecording = reinterpret_cast<const uint32_t*>(scalarData + excSpikeRecordingPtr);
        std::ofstream spikeFile("exc_spikes_sim.csv");
        for(size_t t = 0; t < numTimesteps; t++) {
            AppUtils::writeSpikes(spikeFile, excSpikeRecording, t * 1.0, numExcWords);
            excSpikeRecording += numExcWords;
        }
#endif
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
    return 0;
}
