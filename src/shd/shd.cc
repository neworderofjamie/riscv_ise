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

template<typename T>
std::vector<T> loadData(const std::string &filename)
{
    std::ifstream input(filename, std::ios::binary);

    // Get length
    input.seekg (0, std::ios::end);
    const auto lengthBytes = input.tellg();
    input.seekg (0, std::ios::beg);

    // Check contents is half-word aligned
    assert((lengthBytes % sizeof(T)) == 0);

    // Create vector
    std::vector<T> data(lengthBytes / sizeof(T), 0);

    // Read data directly into it
    input.read(reinterpret_cast<char*>(data.data()), lengthBytes);

    return data;
}

struct StaticPulseTarget
{
    uint32_t weightPtr;
    uint32_t postISynPtr;
    uint32_t numPost;
    uint32_t scaleShift;
};

void genStaticPulse(CodeGenerator &c, RegisterAllocator<VReg> &vectorRegisterAllocator,
                    RegisterAllocator<Reg> &scalarRegisterAllocator,
                    uint32_t preSpikePtr, uint32_t numPre, 
                    const std::vector<StaticPulseTarget> &targets, bool debug)
{
    // Register allocation
    ALLOCATE_SCALAR(SSpikeBuffer);
    ALLOCATE_SCALAR(SSpikeBufferEnd);
    ALLOCATE_SCALAR(SWordNStart);
    ALLOCATE_SCALAR(SConst1);
    ALLOCATE_SCALAR(SSpikeWord);

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
    c.li(*SSpikeBufferEnd, preSpikePtr + ceilDivide(numPre, 32) * 4);

    // Loop through postsynaptic targets
    std::vector<std::pair<std::shared_ptr<ScalarRegisterAllocator::Handle>,
                          std::shared_ptr<ScalarRegisterAllocator::Handle>>> sISynBufferRegs;
    for(const auto &t : targets) {
        // Allocate scalar registers
        auto bufferStartReg = scalarRegisterAllocator.getRegister("SISynBuffer = X");
        auto bufferEndReg = scalarRegisterAllocator.getRegister("SISynBufferEnd = X");

        // Load addresses as immediates
        c.li(*bufferStartReg, t.postISynPtr);
        c.li(*bufferEndReg, t.postISynPtr + ceilDivide(t.numPost, 32) * 64);

        // Add scalar registers to vector
        sISynBufferRegs.emplace_back(bufferStartReg, bufferEndReg);
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

                // SWeightBuffer = weightInHidStart + (numPostVecs * 64 * SN);
                // **TODO** multiply
                ALLOCATE_SCALAR(SWeightBuffer);
                c.li(*SWeightBuffer, t.weightPtr);
                {
                    ALLOCATE_SCALAR(STemp);
                    c.slli(*STemp, *SN, t.scaleShift);
                    c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
                }

                // Reset Isyn pointer
                c.li(*iReg.first, t.postISynPtr);
                
                // Load weight and Isyn
                if(debug) {
                    c.ebreak();
                }

                // Loop over postsynaptic neurons
                if(t.numPost > 32) {
                    // Input postsynaptic neuron loop
                    c.L(weightLoop);
                    {
                        ALLOCATE_VECTOR(VWeight);
                        ALLOCATE_VECTOR(VISyn);

                        // Load next vector of weights and ISyns
                        c.vloadv(*VWeight, *SWeightBuffer);
                        c.vloadv(*VISyn, *iReg.first);
                        c.addi(*SWeightBuffer, *SWeightBuffer, 64);

                        // Add weights to ISyn
                        c.vadd_s(*VISyn, *VISyn, *VWeight);

                        // Write back ISyn and increment SISynBuffer
                        c.vstore(*VISyn, *iReg.first);
                        c.addi(*iReg.first, *iReg.first, 64);

                        // If we haven't reached end of Isyn buffer, loop
                        c.bne(*iReg.first, *iReg.second, weightLoop);
                    }
                }
                // Tail if there are non-POT number of postsynaptic neurons
                if((t.numPost % 32) != 0) {
                    ALLOCATE_SCALAR(SMask);
                    ALLOCATE_VECTOR(VWeight);
                    ALLOCATE_VECTOR(VISyn);
                    ALLOCATE_VECTOR(VISynNew);

                    // Calculate mask for final iteration
                    c.li(*SMask, (1 << (padSize(t.numPost, 32) - t.numPost)) - 1);

                    // Load next vector of weights and ISyns
                    c.vloadv(*VWeight, *SWeightBuffer);
                    c.vloadv(*VISyn, *iReg.first);

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

int main()
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Allocate memory
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Constants
    constexpr uint32_t numInput = 700;
    constexpr uint32_t numHidden = 256;
    constexpr uint32_t numOutput = 20;
    constexpr uint32_t hiddenVFixedPoint = 9;
    constexpr uint32_t hiddenAFixedPoint = 8;
    constexpr uint32_t outFixedPoint = 10;
    constexpr uint32_t numTimesteps = 1170;
    constexpr uint32_t numExamples = 1;//2264;
    constexpr uint32_t numInputSpikeWords = ceilDivide(numInput, 32);
    constexpr uint32_t numHiddenSpikeWords = ceilDivide(numHidden, 32);
    constexpr uint32_t numInputSpikeArrayWords = numInputSpikeWords * numTimesteps;

    // Generate seed
    const uint32_t seedPointer = AppUtils::allocateVectorSeedAndInit(vectorInitData);

    // Load vector data
    const uint32_t weightInHidPtr = AppUtils::loadVectors("99-Conn_Pop0_Pop1-g.bin", vectorInitData);
    const uint32_t weightHidOutPtr = AppUtils::loadVectors("99-Conn_Pop1_Pop2-g.bin", vectorInitData);
    const uint32_t weightHidHidPtr = AppUtils::loadVectors("99-Conn_Pop1_Pop1-g.bin", vectorInitData);
    const uint32_t outputBiasPtr = AppUtils::loadVectors("99-Pop2-Bias.bin", vectorInitData);
    
    // Allocate additional vector arrays
    const uint32_t hiddenIsynPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t hiddenVPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t hiddenAPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t hiddenRefracTimePtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);

    const uint32_t outputIsynPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
    const uint32_t outputVPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
    const uint32_t outputVSumPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
   
    // Allocate scalar arrays
    const uint32_t timestepPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t inputSpikeArrayPtr = AppUtils::allocateScalarAndZero(numInputSpikeArrayWords * 4, scalarInitData);
    const uint32_t inputSpikePtr = AppUtils::allocateScalarAndZero(numInputSpikeWords * 4, scalarInitData);
    const uint32_t hiddenSpikePtr = AppUtils::allocateScalarAndZero(numHiddenSpikeWords * 4, scalarInitData);

    // Load data (this is streamed)
    const auto shdSpikes = loadData<uint32_t>("shd_spikes.bin");
    const auto shdLabels = loadData<int16_t>("shd_labels.bin");

    CodeGenerator c;
    {
        VectorRegisterAllocator vectorRegisterAllocator;
        ScalarRegisterAllocator scalarRegisterAllocator;

        // Load time into scalar register
        ALLOCATE_SCALAR(STime);
        {
            c.li(*STime, timestepPtr);
            c.lw(*STime, *STime);
        }

        // Load RNG seed
        // **TODO** don't do all the time
        {
            ALLOCATE_SCALAR(STmp);
            c.li(*STmp, seedPointer);
            c.vloadr0(*STmp);
            c.vloadr1(*STmp, 64);
        }

        // Loop over time
        Label timeLoop;
        c.L(timeLoop);
        {
            // ---------------------------------------------------------------
            // Input->Hidden synapses
            // ---------------------------------------------------------------
            // 2^9 = 2 bytes * 256 hidden neurons
            genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator,
                           inputSpikePtr, numInput,
                           {{weightInHidPtr, hiddenIsynPtr, numHidden, 9}},
                           false);

            // ---------------------------------------------------------------
            // Hidden->Output and Hidden->Hidden synapses
            // ---------------------------------------------------------------
            // 2^6 = 2 bytes * 32 output neurons (rounded up)
            genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator, 
                           hiddenSpikePtr, numHidden,
                           {{weightHidOutPtr, outputIsynPtr, numOutput, 6},
                            {weightHidHidPtr, hiddenIsynPtr, numHidden, 9}},
                           false);

            // ---------------------------------------------------------------
            // Input neurons
            // ---------------------------------------------------------------
            {
                // Register allocation
                ALLOCATE_SCALAR(SSpikeBuffer);
                ALLOCATE_SCALAR(SSpikeBufferEnd);
                ALLOCATE_SCALAR(SSpikeArrayBuffer);
                ALLOCATE_SCALAR(SSpikeTimeBufferEnd);

                // Labels
                Label neuronLoop;

                // Get address of spike and spike time buffer
                c.li(*SSpikeBuffer, inputSpikePtr);
                c.li(*SSpikeBufferEnd, inputSpikePtr + (numInputSpikeWords * 4));
            
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

                // Input neuron loop
                // **OPTIMIZE** this is technically not necessary at all, could just point point static pulse at buffer
                c.L(neuronLoop);
                {
                    // Register allocation
                    ALLOCATE_SCALAR(SSpikeWord);

                    // Load word from spike array buffer an write to spike buffer
                    c.lw(*SSpikeWord, *SSpikeArrayBuffer);
                    
                    // SSpikeArrayBuffer += 4
                    c.addi(*SSpikeArrayBuffer, *SSpikeArrayBuffer, 4);

                    // inputSpikeBuffer + scalarOffset = spike vector
                    c.sw(*SSpikeWord, *SSpikeBuffer);

                    // SSpikeBuffer += 4
                    c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);
                
                    // If SSpikeBuffer != SSpikeBufferEnd, goto input loop
                    c.bne(*SSpikeBuffer, *SSpikeBufferEnd, neuronLoop);
                }
            }

            // ---------------------------------------------------------------
            // Hidden neurons
            // ---------------------------------------------------------------
            {
                // Register allocation
                ALLOCATE_SCALAR(SVBuffer);
                ALLOCATE_SCALAR(SVBufferEnd);
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
                
                // Labels
                Label neuronLoop;
                
                // Load constants
                // alpha = e^(-1/20)
                c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / 20.0), 14));
                c.vlui(*VRho, convertFixedPoint(std::exp(-1.0 / 2000.0), 14));
                c.vlui(*VBeta, convertFixedPoint(0.0174, hiddenVFixedPoint));
                c.vlui(*VVThresh, convertFixedPoint(0.6, hiddenVFixedPoint));
                c.vlui(*VTauRefrac, 5);
                c.vlui(*VDT, 1);
                c.vlui(*VZero, 0);
                c.vlui(*VOne, convertFixedPoint(1.0, hiddenAFixedPoint));

                // Get address of buffers
                c.li(*SVBuffer, hiddenVPtr);
                c.li(*SVBufferEnd, hiddenVPtr + (numHidden * 2));
                c.li(*SABuffer, hiddenAPtr);
                c.li(*SISynBuffer, hiddenIsynPtr);
                c.li(*SRefracTimeBuffer, hiddenRefracTimePtr);
                c.li(*SSpikeBuffer, hiddenSpikePtr);

                // Hidden neuron loop
                // **NOTE** POT, no tail required
                c.L(neuronLoop);
                {
                    // Register allocation
                    ALLOCATE_VECTOR(VV);
                    ALLOCATE_VECTOR(VA);
                    ALLOCATE_VECTOR(VISyn);
                    ALLOCATE_VECTOR(VRefracTime);
                    ALLOCATE_SCALAR(SSpikeOut);
                    ALLOCATE_SCALAR(SRefractory);

                    // Load state variables
                    c.vloadv(*VV, *SVBuffer);
                    c.vloadv(*VA, *SABuffer);
                    c.vloadv(*VISyn, *SISynBuffer);
                    c.vloadv(*VRefracTime, *SRefracTimeBuffer);

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
                    c.sw(*SSpikeOut, *SSpikeBuffer);

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
                    c.vstore(*VV, *SVBuffer);
                    c.addi(*SVBuffer, *SVBuffer, 64);
                    c.vstore(*VA, *SABuffer);
                    c.addi(*SABuffer, *SABuffer, 64);
                    c.vstore(*VISyn, *SISynBuffer);
                    c.addi(*SISynBuffer, *SISynBuffer, 64);
                    c.vstore(*VRefracTime, *SRefracTimeBuffer);
                    c.addi(*SRefracTimeBuffer, *SRefracTimeBuffer, 64);
                    c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);

                    // If SBBuffer != SVBufferEnd, loop
                    c.bne(*SVBuffer, *SVBufferEnd, neuronLoop);
                }
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

            // End
            c.ecall();
        }

        LOGI << "Max vector registers used: " << vectorRegisterAllocator.getMaxUsedRegisters();
        LOGI << "Max scalar registers used: " << scalarRegisterAllocator.getMaxUsedRegisters();
    }

    // Assemble instructions
    const auto code = c.getCode();
    LOGI << code.size() << " instructions (" << code.size() * 4 << " bytes)";
    LOGI << scalarInitData.size() << " bytes of scalar memory required";
    LOGI << vectorInitData.size() * 2 << " bytes of vector memory required (" << ceilDivide(vectorInitData.size() / 32, 4096) << " URAM cascade)";

    // Create RISC-V core with instruction and scalar data
    RISCV riscV(code, scalarInitData);
    
    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
    // Recording data
    std::vector<uint32_t> inputSpikeRecording;
    std::vector<uint32_t> hiddenSpikeRecording;
    std::vector<int16_t> hiddenVRecording;
    std::vector<int16_t> hiddenARecording;
    std::vector<int16_t> outputVRecording;
    std::vector<int16_t> outputVSumRecording;
    inputSpikeRecording.reserve(numTimesteps * numInputSpikeWords);
    hiddenSpikeRecording.reserve(numTimesteps * numHiddenSpikeWords);
    hiddenVRecording.reserve(numTimesteps * numHidden);
    hiddenARecording.reserve(numTimesteps * numHidden);
    outputVRecording.reserve(numTimesteps * numOutput);
    outputVSumRecording.reserve(numTimesteps * numOutput);

    // Get pointers to scalar and vector memory
    auto *scalarData = riscV.getScalarDataMemory().getData().data();
    auto *vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData().data();

    // From these, get pointers to data structures
    const uint32_t *inputSpikeWords = reinterpret_cast<const uint32_t*>(scalarData + inputSpikePtr);
    const uint32_t *hiddenSpikeWords = reinterpret_cast<const uint32_t*>(scalarData + hiddenSpikePtr);
    const int16_t *hiddenV = vectorData + (hiddenVPtr / 2);
    const int16_t *hiddenA = vectorData + (hiddenAPtr / 2);
    const int16_t *outputV = vectorData + (outputVPtr / 2);
    int16_t *outputVSum = vectorData + (outputVSumPtr / 2);

    // Loop through examples
    int numCorrect = 0;
    for(int i = 0; i < numExamples; i++) {
        // Show % progress
        /*const auto iPerc = std::div(i, (numExamples / 100));
        if(iPerc.rem == 0) {
            std:: cout << iPerc.quot << "%" << std::endl;
        }*/

        // Copy input spike bits into scalar memory
        std::copy_n(shdSpikes.data() + (numInputSpikeArrayWords * i), numInputSpikeArrayWords, 
                    reinterpret_cast<uint32_t*>(scalarData + inputSpikeArrayPtr));

        // Loop through time
        for(uint32_t t = 0; t < numTimesteps; t++) {
            // Copy timestep into scalar memory
            std::memcpy(scalarData + timestepPtr, &t, 4);
        
            // Reset PC and run
            riscV.setPC(0);
            if(!riscV.run()) {
                return 1;
            }

            // Record spike words
            std::copy(inputSpikeWords, inputSpikeWords + numInputSpikeWords,
                      std::back_inserter(inputSpikeRecording));
            std::copy(hiddenSpikeWords, hiddenSpikeWords + numHiddenSpikeWords,
                      std::back_inserter(hiddenSpikeRecording));
            
            // Record state variables
            std::copy_n(hiddenV, numHidden, std::back_inserter(hiddenVRecording));
            std::copy_n(hiddenA, numHidden, std::back_inserter(hiddenARecording));
            std::copy_n(outputV, numOutput, std::back_inserter(outputVRecording));
            std::copy_n(outputVSum, numOutput, std::back_inserter(outputVSumRecording));
        }

        // Determine if output is correct
        const auto classification = std::distance(outputVSum, std::max_element(outputVSum, outputVSum + 10));
        if(classification == shdLabels[i]) {
            numCorrect++;
        }

        // Zero output V sum
        std::fill_n(outputVSum, numOutput, 0);
    }

    std::cout << numCorrect << " / " << numExamples << " correct (" << 100.0 * (numCorrect / (double)numExamples) << "%)" << std::endl;
    std::cout << "Stats:" << std::endl;
    std::cout << "\t" << riscV.getNumInstructionsExecuted() << " instructions executed" << std::endl;
    std::cout << "\t" << riscV.getNumCoprocessorInstructionsExecuted(vectorQuadrant) << " vector instructions executed" << std::endl;
    std::cout << "\t" << riscV.getNumJumps() << " jumps" << std::endl;
    
    // Record output spikes
    std::ofstream inputSpikes("shd_input_spikes.csv");
    std::ofstream hiddenSpikes("shd_hidden_spikes.csv");
    std::ofstream hiddenVFile("shd_hidden_v.csv");
    std::ofstream hiddenAFile("shd_hidden_a.csv");
    std::ofstream outputVFile("shd_output_v.csv");
    std::ofstream outputVSumFile("shd_output_v_sum.csv");
    auto iHV = hiddenVRecording.cbegin();
    auto iHA = hiddenARecording.cbegin();
    auto iOV = outputVRecording.cbegin();
    auto iOVS = outputVSumRecording.cbegin();
    for(uint32_t t = 0; t < numTimesteps; t++) {
        AppUtils::writeSpikes(inputSpikes, inputSpikeRecording.data() + (numInputSpikeWords * t),
                              t, numInputSpikeWords);
        AppUtils::writeSpikes(hiddenSpikes, hiddenSpikeRecording.data() + (numHiddenSpikeWords * t),
                              t, numHiddenSpikeWords);
        
        for(uint32_t i = 0; i < numHidden; i++) {
            hiddenVFile << *iHV++;
            hiddenAFile << *iHA++;
            if(i != (numHidden - 1)) {
                hiddenVFile << ", ";
                hiddenAFile << ", ";
            }
        }
        for(uint32_t i = 0; i < numOutput; i++) {
            outputVFile << *iOV++;
            outputVSumFile << *iOVS++;
            if(i != (numOutput - 1)) {
                outputVFile << ", ";
                outputVSumFile << ", ";
            }
        }
        hiddenVFile << std::endl;
        hiddenAFile << std::endl;
        outputVFile << std::endl;
        outputVSumFile << std::endl;
    }

    
    return 0;
}
