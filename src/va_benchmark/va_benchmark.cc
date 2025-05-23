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

#define RECORD_SPIKES
//#define RECORD_V

// ---------------------------------------------------------------------------
// Anonymous namespace
// ---------------------------------------------------------------------------
namespace
{
struct StaticPulseTarget
{
    uint32_t postIndPtr;
    int16_t weight;
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
    std::vector<ScalarRegisterAllocator::RegisterPtr> targetRegisters;
    for(const auto &t : targets) {
        // Allocate scalar registers
        auto numPostStrideReg = scalarRegisterAllocator.getRegister("SNumPostStride = X");

        // Convert postsynaptic stride to bytes and load immediate
        // **THINK** this seems totally unecessary
        c.li(*numPostStrideReg, t.maxRowLength * 2);

        // Add scalar registers to vector
        targetRegisters.emplace_back(numPostStrideReg);
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
                const auto &byteStrideReg = targetRegisters[i];

                // Multiply presynaptic neuron index by max row length
                ALLOCATE_SCALAR(SPostIndBuffer);
                c.li(*SPostIndBuffer, t.postIndPtr);
                {
                    ALLOCATE_SCALAR(STemp);
                    c.mul(*STemp, *SN, *byteStrideReg);
                    c.add(*SPostIndBuffer, *SPostIndBuffer, *STemp);
                }
                
                // Break if debug is enabled on this target
                if(t.debug) {
                    c.ebreak();
                }

                // Loop over postsynaptic neurons
                ALLOCATE_SCALAR(SMask);
                ALLOCATE_VECTOR(VPostInd1);
                ALLOCATE_VECTOR(VPostInd2);
                ALLOCATE_VECTOR(VAccum1);
                ALLOCATE_VECTOR(VAccum2);
                ALLOCATE_VECTOR(VWeight);
                
                // Preload first index and accumulator
                c.vloadv(*VPostInd1, *SPostIndBuffer);
                c.vlui(*VWeight, (uint16_t)t.weight);
                c.vloadl(*VAccum1, *VPostInd1, t.laneLocalImm);

                AssemblerUtils::unrollVectorLoopBody(
                    c, scalarRegisterAllocator, t.maxRowLength, 4, *SPostIndBuffer,
                    [&t, SMask, SPostIndBuffer, VPostInd1, VPostInd2, VAccum1, VAccum2, VWeight, VZero]
                    (CodeGenerator &c, uint32_t r, bool even, ScalarRegisterAllocator::RegisterPtr maskReg)
                    {
                        assert(!maskReg);

                        // Load vector of postsynaptic indices for next iteration
                        c.vloadv(even ? *VPostInd2 : *VPostInd1,
                                 *SPostIndBuffer, (r + 1) * 64);

                        // Using postsynaptic indices, load accumulator for next iteration
                        c.vloadl(even ? *VAccum2 : *VAccum1, even ? *VPostInd2 : *VPostInd1,
                                 t.laneLocalImm);

                        // Add weights to accumulator loaded in previous iteration
                        auto VAccum = even ? VAccum1 : VAccum2;
                        c.vadd_s(*VAccum, *VAccum, *VWeight);

                        // Write back accumulator
                        c.vstorel(*VAccum, even ? *VPostInd1 : *VPostInd2, t.laneLocalImm);
                    },
                    [SPostIndBuffer]
                    (CodeGenerator &c, uint32_t numUnrolls)
                    {
                        // Increment pointers 
                        c.addi(*SPostIndBuffer, *SPostIndBuffer, 64 * numUnrolls);
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
// ---------------------------------------------------------------------------
void genLIF(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
            ScalarRegisterAllocator &scalarRegisterAllocator,
            uint32_t numNeurons,
            uint32_t vPtr, uint32_t refracTimePtr, uint32_t spikeArrayPtr,
            uint32_t eLLPtr, uint32_t iLLPtr,
            uint32_t fixedPoint = 8,
            ScalarRegisterAllocator::RegisterPtr spikeRecordingBuffer = nullptr,
            ScalarRegisterAllocator::RegisterPtr vRecordingBuffer = nullptr,
            double tauM = 20.0, double tauSynExc = 5.0, double tauSynInh = 10.0,
            double vThresh = 10.0, double iOffset = 0.55)
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

    const uint32_t numNeuronWords = ceilDivide(numNeurons, 32);

    // Load constants
    c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / tauM), 14));
    c.vlui(*VEBeta, convertFixedPoint(std::exp(-1.0 / tauSynExc), 14));
    c.vlui(*VIBeta, convertFixedPoint(std::exp(-1.0 / tauSynInh), 14));
    c.vlui(*VEScale, convertFixedPoint(tauSynExc * (1.0 - std::exp(-1.0 / tauSynExc)), fixedPoint));
    c.vlui(*VIScale, convertFixedPoint(tauSynInh * (1.0 - std::exp(-1.0 / tauSynInh)), fixedPoint));
    c.vlui(*VThresh, convertFixedPoint(vThresh, fixedPoint));
    c.vlui(*VIOffset, convertFixedPoint(iOffset, fixedPoint));
    c.vlui(*VRMembrane, convertFixedPoint(tauM / 1.0, fixedPoint));
    c.vlui(*VTauRefrac, 5);
    c.vlui(*VDT, 1);
    c.vlui(*VZero, 0);
    c.vlui(*VSynLLOffset, 0);
    c.vlui(*VNumUnrollBytes, 2 * std::min(numNeuronWords, 4u));

    // Get address of buffers
    c.li(*SVBuffer, vPtr);
    c.li(*SRefracTimeBuffer, refracTimePtr);
    c.li(*SSpikeBuffer, spikeArrayPtr);
 
    AssemblerUtils::unrollVectorLoopBody(
        c, scalarRegisterAllocator, numNeurons, 4, *SVBuffer,
        [&scalarRegisterAllocator, &vectorRegisterAllocator,
         fixedPoint, eLLPtr, iLLPtr,
         SVBuffer, SRefracTimeBuffer, SSpikeBuffer,
         spikeRecordingBuffer, vRecordingBuffer,
         VAlpha, VEBeta, VIBeta, VEScale, VIScale, VDT, VRMembrane, VSynLLOffset, VTauRefrac, VThresh, VIOffset, VZero]
        (CodeGenerator &c, uint32_t r, bool, ScalarRegisterAllocator::RegisterPtr maskReg)
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
            c.vloadl(*VESyn, *VSynLLOffset, eLLPtr + (r * 2));
            c.vloadl(*VISyn, *VSynLLOffset, iLLPtr + (r * 2));
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
                c.vmul_rs(fixedPoint, *VTmp, *VISyn, *VIScale);
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
                c.vsub_s(*VVTemp, *VAlphaTemp, *VV);
                c.vmul_rs(14, *VVTemp, *VVTemp, *VAlpha);

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

            // VV = SSpikeOut ? VZero : VV
            c.vsel(*VV, *SSpikeOut, *VZero);

            // VRefracTime = SSpikeOut ? VTauRefrac : VRefracTime
            c.vsel(*VRefracTime, *SSpikeOut, *VTauRefrac);
            
            // Record spikes
            if(spikeRecordingBuffer) {
                c.sw(*SSpikeOut, *spikeRecordingBuffer);
                c.addi(*spikeRecordingBuffer, *spikeRecordingBuffer, 4);
            }

            // Record v of first neuron
            /*if(vRecordingBuffer && i == 0) {
                ALLOCATE_SCALAR(STmp);
                c.vextract(*STmp, *VV, 0);
                c.sh(*STmp, *vRecordingBuffer);
                c.addi(*vRecordingBuffer, *vRecordingBuffer, 2);
            }*/

            // Store VV and refrac time and increment buffers
            c.vstore(*VV, *SVBuffer, 64 * r);
            c.vstore(*VRefracTime, *SRefracTimeBuffer, 64 * r);

            // Store VESyn and VIsyn back to lane-local memory
            c.vstorel(*VESyn, *VSynLLOffset, eLLPtr + (r * 2));
            c.vstorel(*VISyn, *VSynLLOffset, iLLPtr + (r * 2));
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
// ---------------------------------------------------------------------------
void writeSpikes(const char *filename, const volatile uint32_t *recordingData,
                 uint32_t numTimesteps, uint32_t numWords)
{
    std::ofstream spikeFile(filename);
    for(size_t t = 0; t < numTimesteps; t++) {
        AppUtils::writeSpikes(spikeFile, recordingData, t * 1.0, numWords);
        recordingData += numWords;
    }
}
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
    constexpr uint32_t fixedPoint = 10;
    constexpr uint32_t numExc = 410;
    constexpr uint32_t numInh = 102;
    constexpr uint32_t numExcWords = ceilDivide(numExc, 32);
    constexpr uint32_t numInhWords = ceilDivide(numInh, 32);
    constexpr uint32_t numExcIncomingVectors = 7;
    constexpr uint32_t numInhIncomingVectors = 3;
    const uint32_t numExcSpikeRecordingWords = numExcWords * numTimesteps;
    const uint32_t numInhSpikeRecordingWords = numInhWords * numTimesteps;

    // Layout lane local memory
    constexpr uint32_t eeLLAddr = 2;
    constexpr uint32_t eiLLAddr = eeLLAddr + (numExcWords * 2);
    constexpr uint32_t iiLLAddr = eiLLAddr + (numInhWords * 2);
    constexpr uint32_t ieLLAddr = iiLLAddr + (numInhWords * 2);

    // Allocate vector arrays
    const uint32_t seedPtr = AppUtils::allocateVectorSeedAndInit(vectorInitData);
    const uint32_t eeIndPtr = AppUtils::loadVectors("va_benchmark_ee.bin", vectorInitData);
    const uint32_t eiIndPtr = AppUtils::loadVectors("va_benchmark_ei.bin", vectorInitData);
    const uint32_t iiIndPtr = AppUtils::loadVectors("va_benchmark_ii.bin", vectorInitData);
    const uint32_t ieIndPtr = AppUtils::loadVectors("va_benchmark_ie.bin", vectorInitData);
    const uint32_t indPadPtr = AppUtils::allocateVectorAndZero(32, vectorInitData);
    const uint32_t excVPtr = AppUtils::allocateVectorAndZero(numExc, vectorInitData);
    const uint32_t excRefracTimePtr = AppUtils::allocateVectorAndZero(numExc, vectorInitData);

    const uint32_t inhVPtr = AppUtils::allocateVectorAndZero(numInh, vectorInitData);
    const uint32_t inhRefracTimePtr = AppUtils::allocateVectorAndZero(numInh, vectorInitData);
    
    // Allocate scalar arrays
    const uint32_t excSpikeArrayPtr = AppUtils::allocateScalarAndZero(numExcWords * 4, scalarInitData);
    const uint32_t inhSpikeArrayPtr = AppUtils::allocateScalarAndZero(numInhWords * 4, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

#ifdef RECORD_SPIKES
     const uint32_t excSpikeRecordingPtr = AppUtils::allocateScalarAndZero(numExcSpikeRecordingWords * 4, scalarInitData);
     const uint32_t inhSpikeRecordingPtr = AppUtils::allocateScalarAndZero(numInhSpikeRecordingWords * 4, scalarInitData);
#endif
#ifdef RECORD_V
    const uint32_t excVRecordingPtr = AppUtils::allocateScalarAndZero(2 * numTimesteps, scalarInitData);
#endif

    // Increase scalar memory for buffering
    assert(scalarInitData.size() <= (128 * 1024));
    scalarInitData.resize(128 * 1024, 0);

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
                ALLOCATE_SCALAR(SSpikeArrayBuffer);
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
                c.li(*SSpikeArrayBuffer, excSpikeArrayPtr);

                // Excitatory neuron loop
                // **NOTE** all these allocations are padded so we overfill to simplify code
                AssemblerUtils::unrollVectorLoopBody(
                    c, scalarRegisterAllocator, numExcWords * 32, 4, *SVBuffer,
                    [&scalarRegisterAllocator, &vectorRegisterAllocator,
                    fixedPoint, eeLLAddr, eiLLAddr,
                    SRefracTimeBuffer, SVBuffer, SSpikeArrayBuffer, VRandom, VScale, VSynLLOffset, VZero]
                    (CodeGenerator &c, uint32_t r, uint32_t, ScalarRegisterAllocator::RegisterPtr)
                    {
                        // Generate initial mebrane voltages from U(0,Scale) 
                        c.vrng(*VRandom);
                        c.vmul(15, *VRandom, *VRandom, *VScale);

                        c.vstorel(*VZero, *VSynLLOffset, eeLLAddr + (r * 2));
                        c.vstorel(*VZero, *VSynLLOffset, eiLLAddr + (r * 2));
                        c.vstore(*VRandom, *SVBuffer, r * 64);
                        c.vstore(*VZero, *SRefracTimeBuffer, r * 64);
                        c.sw(Reg::X0, *SSpikeArrayBuffer, r * 4);
                    },
                    [SRefracTimeBuffer, SVBuffer, SSpikeArrayBuffer, VNumUnrollBytes, VSynLLOffset]
                    (CodeGenerator &c, uint32_t numUnrolls)
                    {
                        c.addi(*SVBuffer, *SVBuffer, 64 * numUnrolls);
                        c.addi(*SRefracTimeBuffer, *SRefracTimeBuffer, 64 * numUnrolls);
                        c.addi(*SSpikeArrayBuffer, *SSpikeArrayBuffer, 4);
                        c.vadd(*VSynLLOffset, *VSynLLOffset, *VNumUnrollBytes);
                    });
            }

            // ---------------------------------------------------------------
            // Inh neurons
            // ---------------------------------------------------------------
            {
                // Register allocation
                ALLOCATE_SCALAR(SVBuffer);
                ALLOCATE_SCALAR(SRefracTimeBuffer);
                ALLOCATE_SCALAR(SSpikeArrayBuffer);
                ALLOCATE_VECTOR(VZero);
                ALLOCATE_VECTOR(VScale);
                ALLOCATE_VECTOR(VRandom);
                ALLOCATE_VECTOR(VSynLLOffset);
                ALLOCATE_VECTOR(VNumUnrollBytes);
                
                // Load constants
                c.vlui(*VZero, 0);
                c.vlui(*VScale, convertFixedPoint(10.0, fixedPoint));
                c.vlui(*VSynLLOffset, 0);
                c.vlui(*VNumUnrollBytes, 2 * std::min(numInhWords, 4u));

                // Get address of buffers
                c.li(*SVBuffer, inhVPtr);
                c.li(*SRefracTimeBuffer, inhRefracTimePtr);
                c.li(*SSpikeArrayBuffer, inhSpikeArrayPtr);

                // Excitatory neuron loop
                // **NOTE** all these allocations are padded so we overfill to simplify code
                AssemblerUtils::unrollVectorLoopBody(
                    c, scalarRegisterAllocator, numInhWords * 32, 4, *SVBuffer,
                    [&scalarRegisterAllocator, &vectorRegisterAllocator,
                    fixedPoint, eiLLAddr, iiLLAddr,
                    SRefracTimeBuffer, SVBuffer, SSpikeArrayBuffer, VRandom, VScale, VSynLLOffset, VZero]
                    (CodeGenerator &c, uint32_t r, uint32_t, ScalarRegisterAllocator::RegisterPtr)
                    {
                        // Generate initial mebrane voltages from U(0,Scale) 
                        c.vrng(*VRandom);
                        c.vmul(15, *VRandom, *VRandom, *VScale);

                        c.vstorel(*VZero, *VSynLLOffset, eiLLAddr + (r * 2));
                        c.vstorel(*VZero, *VSynLLOffset, iiLLAddr + (r * 2));
                        c.vstore(*VRandom, *SVBuffer, r * 64);
                        c.vstore(*VZero, *SRefracTimeBuffer, r * 64);
                        c.sw(Reg::X0, *SSpikeArrayBuffer, r * 4);
                    },
                    [SRefracTimeBuffer, SVBuffer, SSpikeArrayBuffer, VNumUnrollBytes, VSynLLOffset]
                    (CodeGenerator &c, uint32_t numUnrolls)
                    {
                        c.addi(*SVBuffer, *SVBuffer, 64 * numUnrolls);
                        c.addi(*SRefracTimeBuffer, *SRefracTimeBuffer, 64 * numUnrolls);
                        c.addi(*SSpikeArrayBuffer, *SSpikeArrayBuffer, 4);
                        c.vadd(*VSynLLOffset, *VSynLLOffset, *VNumUnrollBytes);
                    });
            }

            // ---------------------------------------------------------------
            // Padding
            // ---------------------------------------------------------------
            // **YUCK** this seems the easiest way to follow connectivity with one vector of -2
            {
                ALLOCATE_SCALAR(SIndPad);
                ALLOCATE_VECTOR(VPad);

                c.li(*SIndPad, indPadPtr);

                c.vlui(*VPad, (uint16_t)-2);

                c.vstore(*VPad, *SIndPad);
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
            ALLOCATE_SCALAR(SInhSpikeRecordingBuffer);
#endif
#ifdef RECORD_V
            ALLOCATE_SCALAR(SExcVRecordingBuffer);
#endif
            // Labels
            Label timeLoop;
            Label spinLoop;

            // Set timestep range
            c.li(*STime, 0);
            c.li(*STimeEnd, numTimesteps);
#ifdef RECORD_SPIKES
            c.li(*SExcSpikeRecordingBuffer, excSpikeRecordingPtr);
            c.li(*SInhSpikeRecordingBuffer, inhSpikeRecordingPtr);
#endif
#ifdef RECORD_V
            c.li(*SExcVRecordingBuffer, excVRecordingPtr);
#endif
            // Loop over time
            c.L(timeLoop);
            {

                // ---------------------------------------------------------------
                // Inhibitory synapses
                // ---------------------------------------------------------------
                const int16_t inhWeight = convertFixedPoint(-0.07968749999999998, fixedPoint);
                genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator,
                               inhSpikeArrayPtr, numInh,
                               {{iiIndPtr, inhWeight, numInhIncomingVectors * 32, iiLLAddr, false},
                                {ieIndPtr, inhWeight, numExcIncomingVectors * 32, ieLLAddr, false}});
                
                // ----------------------------------   -----------------------------
                // Excitatory synapses
                // ---------------------------------------------------------------
                const int16_t excWeight = convertFixedPoint(0.0062499999999999995, fixedPoint);
                genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator,
                               excSpikeArrayPtr, numExc,
                               {{eiIndPtr, excWeight, numInhIncomingVectors * 32, eiLLAddr, false},
                                {eeIndPtr, excWeight, numExcIncomingVectors * 32, eeLLAddr, false}});

                // ---------------------------------------------------------------
                // Excitatory neurons
                // ---------------------------------------------------------------
                genLIF(c, vectorRegisterAllocator, scalarRegisterAllocator,
                       numExc, excVPtr, excRefracTimePtr, excSpikeArrayPtr,
                       eeLLAddr, ieLLAddr, fixedPoint
#ifdef RECORD_SPIKES
                       ,SExcSpikeRecordingBuffer
#endif
#ifdef RECORD_V
                       ,SExcVRecordingBuffer
#endif
                    );
                
                // ---------------------------------------------------------------
                // Inhibitory neurons
                // ---------------------------------------------------------------
                genLIF(c, vectorRegisterAllocator, scalarRegisterAllocator,
                       numInh, inhVPtr, inhRefracTimePtr, inhSpikeArrayPtr,
                       eiLLAddr, iiLLAddr, fixedPoint
#ifdef RECORD_SPIKES
                       ,SInhSpikeRecordingBuffer
#endif
#ifdef RECORD_V
                       ,SInhVRecordingBuffer
#endif
                 );

                c.addi(*STime, *STime, 1);
                c.bne(*STime, *STimeEnd, timeLoop);
            }
        });
    LOGI << simCode.size() << " simulation instructions";
    LOGI << scalarInitData.size() << " bytes of scalar memory required";
    LOGI << vectorInitData.size() * 2 << " bytes of vector memory required (" << ceilDivide(vectorInitData.size() / 32, 4096) << " URAM cascade)";

    if(device) {
        LOGI << "Creating device";
        Device device;

        // Put core into reset state
        LOGI << "Resetting";
        device.setEnabled(false);
        
        LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
        device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());

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
        std::cout << "Startup time:" << (initStartTime - programStartTime).count() << " seconds" << std::endl;
        std::cout << "Init time:" << (simStartTime - initStartTime).count() << " seconds" << std::endl;
        std::cout << "Simulation time:" << (simEndTime - simStartTime).count() << " seconds" << std::endl;
        
#ifdef RECORD_SPIKES
        const volatile uint32_t *excSpikeRecording = reinterpret_cast<const volatile uint32_t*>(device.getDataMemory() + excSpikeRecordingPtr);
        writeSpikes("exc_spikes_sim.csv", excSpikeRecording,
                    numTimesteps, numExcWords);
        
        const volatile uint32_t *inhSpikeRecording = reinterpret_cast<const volatile uint32_t*>(device.getDataMemory() + inhSpikeRecordingPtr);
        writeSpikes("inh_spikes_sim.csv", inhSpikeRecording,
                    numTimesteps, numInhWords);
#endif
#ifdef RECORD_V
        const volatile int16_t *excVRecording = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + excVRecordingPtr);
        std::ofstream vFile("exc_v_sim.csv");
        for(size_t t = 0; t < numTimesteps; t++) {
            vFile << *excVRecording++ << std::endl;
        }
#endif
    }
    else {
        // Build ISE with vector co-processor
        RISCV riscV;
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);
        
        // Set instructions and init data
        riscV.setInstructions(initCode);
        riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().setData(vectorInitData);
        riscV.getScalarDataMemory().setData(scalarInitData);
    
        // Run RISC-V to initialize
        riscV.setPC(0);
        if(!riscV.run()) {
            return 1;
        }

        // Reset stats for simulations
        riscV.resetStats();

        // Load simulation program
        riscV.setInstructions(simCode);

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
        
        auto *scalarData = riscV.getScalarDataMemory().getData();
#ifdef RECORD_SPIKES
        const uint32_t *excSpikeRecording = reinterpret_cast<const uint32_t*>(scalarData + excSpikeRecordingPtr);
        writeSpikes("exc_spikes_sim.csv", excSpikeRecording,
                    numTimesteps, numExcWords);
        
        const uint32_t *inhSpikeRecording = reinterpret_cast<const uint32_t*>(scalarData + inhSpikeRecordingPtr);
        writeSpikes("inh_spikes_sim.csv", inhSpikeRecording,
                    numTimesteps, numInhWords);
#endif
#ifdef RECORD_V
        const int16_t *excVRecording = reinterpret_cast<const int16_t*>(scalarData + excVRecordingPtr);
        std::ofstream vFile("exc_v_sim.csv");
        for(size_t t = 0; t < numTimesteps; t++) {
            vFile << *excVRecording++ << std::endl;
        }
#endif
    }
    return 0;
}
