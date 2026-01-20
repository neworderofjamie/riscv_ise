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
#include "ise/router_sim.h"
#include "ise/shared_bus_sim.h"
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
                    Reg spikeReg, const std::vector<StaticPulseTarget> &targets)
{   
    // Mask out neuron ID from neuron
    // **NOTE** it's fine to trash spikeReg
    {
        ALLOCATE_SCALAR(STemp);
        c.li(*STemp, (1 << 18) - 1);
        c.and_(spikeReg, spikeReg, *STemp);
    }

    // Loop through postsynaptic targets
    for(size_t i = 0; i < targets.size(); i++) {
        const auto &t = targets[i];

        // Multiply presynaptic neuron index by max row length
        ALLOCATE_SCALAR(SPostIndBuffer);
        c.li(*SPostIndBuffer, t.postIndPtr);
        {
            ALLOCATE_SCALAR(STemp);

            // Multiply by stride to get address
            c.li(*STemp, t.maxRowLength * 2);
            c.mul(*STemp, spikeReg, *STemp);

            // Add to buffer to get address
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
            [&t, SMask, SPostIndBuffer, VPostInd1, VPostInd2, VAccum1, VAccum2, VWeight]
            (CodeGenerator &c, uint32_t r, bool even, ScalarRegisterAllocator::RegisterPtr maskReg)
            {
                assert(!maskReg);

                // Load vector of postsynaptic indices for next iteration
                c.vloadv(even ? *VPostInd2 : *VPostInd1,
                            *SPostIndBuffer, (r + 1) * 64);

                // Add weights to accumulator loaded in previous iteration
                auto VAccum = even ? VAccum1 : VAccum2;
                c.vadd_s(*VAccum, *VAccum, *VWeight);
                        
                // Using postsynaptic indices, load accumulator for next iteration
                c.vloadl(even ? *VAccum2 : *VAccum1, even ? *VPostInd2 : *VPostInd1,
                         t.laneLocalImm);

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

}
// ---------------------------------------------------------------------------
void genLIF(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
            ScalarRegisterAllocator &scalarRegisterAllocator,
            uint32_t numNeurons,
            uint32_t vPtr, uint32_t refracTimePtr, uint32_t baseSpikeAddress,
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
    ALLOCATE_SCALAR(SBaseSpikeAddress);
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
    c.li(*SBaseSpikeAddress, baseSpikeAddress);

    AssemblerUtils::unrollVectorLoopBody(
        c, scalarRegisterAllocator, numNeurons, 4, *SVBuffer,
        [&scalarRegisterAllocator, &vectorRegisterAllocator,
         fixedPoint, eLLPtr, iLLPtr,
         SVBuffer, SRefracTimeBuffer, SBaseSpikeAddress,
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
            // Set router base address
            // **YUCK** not unrolling friendly
            c.csrw(CSR::MASTER_EVENT_ID_BASE, *SBaseSpikeAddress);
            c.csrw(CSR::MASTER_EVENT_BITFIELD, *SSpikeOut);
            c.addi(*SBaseSpikeAddress, *SBaseSpikeAddress, 32 /* * numCores*/);

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
        [SRefracTimeBuffer, SVBuffer, VNumUnrollBytes, VSynLLOffset]
        (CodeGenerator &c, uint32_t numUnrolls)
        {
            c.addi(*SVBuffer, *SVBuffer, 64 * numUnrolls);
            c.addi(*SRefracTimeBuffer, *SRefracTimeBuffer, 64 * numUnrolls);
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
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

#ifdef RECORD_SPIKES
     const uint32_t excSpikeRecordingPtr = AppUtils::allocateScalarAndZero(numExcSpikeRecordingWords * 4, scalarInitData);
     const uint32_t inhSpikeRecordingPtr = AppUtils::allocateScalarAndZero(numInhSpikeRecordingWords * 4, scalarInitData);
#endif
#ifdef RECORD_V
    const uint32_t excVRecordingPtr = AppUtils::allocateScalarAndZero(2 * numTimesteps, scalarInitData);
#endif

    // Create spike buffer at start of spike memory
    const uint32_t spikeBufferPtr = 31 * 4096;

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
                    fixedPoint, eeLLAddr, ieLLAddr,
                    SRefracTimeBuffer, SVBuffer, VRandom, VScale, VSynLLOffset, VZero]
                    (CodeGenerator &c, uint32_t r, uint32_t, ScalarRegisterAllocator::RegisterPtr)
                    {
                        // Generate initial mebrane voltages from U(0,Scale) 
                        c.vrng(*VRandom);
                        c.vmul(15, *VRandom, *VRandom, *VScale);

                        c.vstorel(*VZero, *VSynLLOffset, eeLLAddr + (r * 2));
                        c.vstorel(*VZero, *VSynLLOffset, ieLLAddr + (r * 2));
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

            // ---------------------------------------------------------------
            // Inh neurons
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
                c.vlui(*VNumUnrollBytes, 2 * std::min(numInhWords, 4u));

                // Get address of buffers
                c.li(*SVBuffer, inhVPtr);
                c.li(*SRefracTimeBuffer, inhRefracTimePtr);

                // Excitatory neuron loop
                // **NOTE** all these allocations are padded so we overfill to simplify code
                AssemblerUtils::unrollVectorLoopBody(
                    c, scalarRegisterAllocator, numInhWords * 32, 4, *SVBuffer,
                    [&scalarRegisterAllocator, &vectorRegisterAllocator,
                    fixedPoint, eiLLAddr, iiLLAddr,
                    SRefracTimeBuffer, SVBuffer, VRandom, VScale, VSynLLOffset, VZero]
                    (CodeGenerator &c, uint32_t r, uint32_t, ScalarRegisterAllocator::RegisterPtr)
                    {
                        // Generate initial mebrane voltages from U(0,Scale) 
                        c.vrng(*VRandom);
                        c.vmul(15, *VRandom, *VRandom, *VScale);

                        c.vstorel(*VZero, *VSynLLOffset, eiLLAddr + (r * 2));
                        c.vstorel(*VZero, *VSynLLOffset, iiLLAddr + (r * 2));
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
            ALLOCATE_SCALAR(SSpike);

            ALLOCATE_SCALAR(SSpikeBuffer);
            ALLOCATE_SCALAR(SSpikeBufferStart);
            ALLOCATE_SCALAR(SSpikeBufferEnd);
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
            Label jumpTable;
            Label rowReturn;
            Label excRow;
            Label inhRow;
            Label start;
            Label nextSpike;
            
            // Jump over jump table etc to start
            // **TODO** configurable start and interrupt addresses
            c.j_(start);

            // Define jump table for 
            c.L(jumpTable);
            c.j_(excRow);
            c.j_(inhRow);

            // ----------------------------------   -----------------------------
            // Excitatory synapses
            // ---------------------------------------------------------------
            {
                c.L(excRow);
                
                const int16_t excWeight = convertFixedPoint(0.0062499999999999995, fixedPoint);
                genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator, *SSpike,
                               {{eiIndPtr, excWeight, numInhIncomingVectors * 32, eiLLAddr, false},
                                {eeIndPtr, excWeight, numExcIncomingVectors * 32, eeLLAddr, false}});
                c.j_(nextSpike);
            }

            // ---------------------------------------------------------------
            // Inhibitory synapses
            // ---------------------------------------------------------------
            {
                c.L(inhRow);
                const int16_t inhWeight = convertFixedPoint(-0.07968749999999998, fixedPoint);
                genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator, *SSpike,
                               {{iiIndPtr, inhWeight, numInhIncomingVectors * 32, iiLLAddr, false},
                                {ieIndPtr, inhWeight, numExcIncomingVectors * 32, ieLLAddr, false}});
                c.j_(nextSpike);
            }

            // Set timestep range
            c.L(start);
            c.li(*STime, 0);
            c.li(*STimeEnd, numTimesteps);
#ifdef RECORD_SPIKES
            c.li(*SExcSpikeRecordingBuffer, excSpikeRecordingPtr);
            c.li(*SInhSpikeRecordingBuffer, inhSpikeRecordingPtr);
#endif
#ifdef RECORD_V
            c.li(*SExcVRecordingBuffer, excVRecordingPtr);
#endif
            // Reset router slave to start writing at beginning of spike buffer
            c.li(*SSpikeBufferStart, spikeBufferPtr);
            c.csrw(CSR::SLAVE_EVENT_ADDRESS, *SSpikeBufferStart);

            // Loop over time
            c.L(timeLoop);
            {
                {
                    Label spikeLoop;
                    Label spikeLoopEnd;

                    // Load start and end of this timestep's spike buffer
                    c.mv(*SSpikeBuffer, *SSpikeBufferStart);
                    c.csrr(*SSpikeBufferEnd, CSR::SLAVE_EVENT_ADDRESS);

                    // While (spikeBuffer != spikeBufferEnd
                    c.L(spikeLoop);
                    c.beq(*SSpikeBuffer, *SSpikeBufferEnd, spikeLoopEnd);
                    {
                        // Load spike from buffer
                        c.lw(*SSpike, *SSpikeBuffer);

                        {
                            // Extract population ID
                            ALLOCATE_SCALAR(SPopulationID);
                            c.srli(*SPopulationID, *SSpike, 19);

                            // Jump to correct population handler
                            c.jalr(Reg::X0, *SPopulationID, jumpTable.getAddress());
                        }
                        c.L(nextSpike);

                        // Loop until spikes are processed
                        c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);
                        c.j_(spikeLoop);
                    }
                    c.L(spikeLoopEnd);
                }
               
                // Reset router slave to start writing at beginning of spike buffer
                c.csrw(CSR::SLAVE_EVENT_ADDRESS, *SSpikeBufferStart);

                // Wait for all routers to be reset
                AssemblerUtils::generateRouterBarrier(c, scalarRegisterAllocator, 1);

                // ---------------------------------------------------------------
                // Excitatory neurons
                // ---------------------------------------------------------------
                genLIF(c, vectorRegisterAllocator, scalarRegisterAllocator,
                       numExc, excVPtr, excRefracTimePtr, (0 << 19),
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
                       numInh, inhVPtr, inhRefracTimePtr, (4 << 19),
                       eiLLAddr, iiLLAddr, fixedPoint
#ifdef RECORD_SPIKES
                       ,SInhSpikeRecordingBuffer
#endif
#ifdef RECORD_V
                       ,SInhVRecordingBuffer
#endif
                       );

                // Wait for all events to be communicated
                AssemblerUtils::generateRouterBarrier(c, scalarRegisterAllocator, 1);

                c.addi(*STime, *STime, 1);
                c.bne(*STime, *STimeEnd, timeLoop);
            }
        });
    LOGI << simCode.size() << " simulation instructions";
    LOGI << scalarInitData.size() << " bytes of scalar memory required";
    LOGI << vectorInitData.size() * 2 << " bytes of vector memory required (" << ceilDivide(vectorInitData.size() / 32, 4096) << " URAM cascade)";

    AppUtils::dumpCOE("va_benchmark_sim.coe", simCode);
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
        // Create simulated shared bus to connect the cores
        SharedBusSim sharedBus(1);

        // Build ISE with vector co-processor
        RISCV riscV;
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);

        // Create simulated DMA controller
        RouterSim router(sharedBus, riscV.getSpikeDataMemory(), 0);
        riscV.setRouter(&router);

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
