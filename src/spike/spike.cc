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

struct StaticPulseTarget
{
    uint32_t weightPtr;
    uint32_t postISynPtr;
    uint32_t numPost;
    uint32_t scaleShift;
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
    std::vector<std::pair<std::shared_ptr<ScalarRegisterAllocator::Handle>,
                          std::shared_ptr<ScalarRegisterAllocator::Handle>>> sISynBufferRegs;
    for(const auto &t : targets) {
        // Allocate scalar registers
        auto bufferStartReg = scalarRegisterAllocator.getRegister("SISynBuffer = X");
        auto bufferEndReg = scalarRegisterAllocator.getRegister("SISynBufferEnd = X");

        // Load addresses as immediates
        // **NOTE** end address is only end of loop body not tail
        c.li(*bufferStartReg, t.postISynPtr);
        c.li(*bufferEndReg, t.postISynPtr + (t.numPost / 32) * 64);

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
                if(t.debug) {
                    c.ebreak();
                }

                // Loop over postsynaptic neurons
                if(t.numPost > 32) {
                    ALLOCATE_VECTOR(VWeight);
                    ALLOCATE_VECTOR(VISyn1);
                    ALLOCATE_VECTOR(VISyn2);

                    // Preload first ISyn to avoid stall
                    c.vloadv(*VISyn1, *iReg.first, 0);

                    AssemblerUtils::unrollVectorLoopBody(
                        c, t.numPost, 4, *iReg.first, *iReg.second,
                        [&iReg, SWeightBuffer, VWeight, VISyn1, VISyn2]
                        (CodeGenerator &c, uint32_t r)
                        {
                            // Load vector of weights
                            c.vloadv(*VWeight, *SWeightBuffer, r * 64);

                            // Load NEXT vector of ISyn to avoid stall
                            // **YUCK** in last iteration, while this may not be accessed, it may be out of bounds
                            const bool even = ((r % 2) == 0);                            
                            c.vloadv(even ? *VISyn2 : *VISyn1, *iReg.first, (r + 1) * 64);
                            
                            // Add weights to ISyn
                            auto VISyn = even ? VISyn1 : VISyn2;
                            c.vadd_s(*VISyn, *VISyn, *VWeight);

                            // Write back ISyn and increment SISynBuffer
                            c.vstore(*VISyn, *iReg.first, r * 64);
                        },
                        [&iReg, SWeightBuffer]
                        (CodeGenerator &c, uint32_t numUnrolls)
                        {
                            // Increment pointers 
                            c.addi(*iReg.first, *iReg.first, 64 * numUnrolls);
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
                    c.vloadv(*VISyn, *iReg.first);
                    
                    // **STALL**
                    c.nop();
                    
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

void check(const volatile int16_t *hiddenIsyn, size_t numHidden)
{
    
    for(size_t i = 0; i < numHidden; i++) {
        std::cout << hiddenIsyn[i] << ", ";
    }
    std::cout << std::endl;
   
}

std::vector<uint32_t> generateSimCode(bool simulate, uint32_t numInput, uint32_t numHidden, uint32_t numInputSpikeWords, uint32_t numHiddenSpikeWords, 
                                      uint32_t inputSpikePtr, uint32_t weightInHidPtr, uint32_t hiddenIsynPtr, uint32_t hiddenIsynScalarPtr, 
                                      uint32_t readyFlagPtr)
{
    return AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            
            {
                ALLOCATE_SCALAR(STmp);
                ALLOCATE_VECTOR(VZero);
                c.vlui(*VZero, 0);
                c.li(*STmp, hiddenIsynPtr);
                
                c.vstore(*VZero, *STmp);
                c.vstore(*VZero, *STmp, 64);
                c.vstore(*VZero, *STmp, 128);
                c.vstore(*VZero, *STmp, 192);
            }

            // 2^6 = 2 bytes * 32 hidden neurons
            genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator,
                           inputSpikePtr, numInput,
                           {{weightInHidPtr, hiddenIsynPtr, numHidden, 9, false}});

            // Copy Isyn to BRAM
            AssemblerUtils::generateVectorScalarMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       hiddenIsynPtr, hiddenIsynScalarPtr, numHiddenSpikeWords);
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
    constexpr bool simulate = false;
    constexpr uint32_t numInput = 700;
    constexpr uint32_t numHidden = 256;
    constexpr uint32_t numOutput = 20;
    constexpr uint32_t numInputSpikeWords = ceilDivide(numInput, 32);
    constexpr uint32_t numHiddenSpikeWords = ceilDivide(numHidden, 32);
    constexpr uint32_t numOutputSpikeWords = ceilDivide(numOutput, 32);
    
    // Allocate vector arrays
    // **NOTE** adjacent so can be block copied from scalar
    const uint32_t weightInHidPtr = AppUtils::allocateVectorAndZero(numInput * numHiddenSpikeWords * 32, vectorInitData);
    const uint32_t weightHidOutPtr = AppUtils::allocateVectorAndZero(numHidden * numOutputSpikeWords * 32, vectorInitData);
    const uint32_t weightHidHidPtr = AppUtils::allocateVectorAndZero(numHidden * numHiddenSpikeWords * 32, vectorInitData);
    const uint32_t outputBiasPtr = AppUtils::allocateVectorAndZero(numOutput, vectorInitData);
    const uint32_t hiddenIsynPtr = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t dummy = AppUtils::allocateVectorAndZero(numHidden, vectorInitData);
    
    // Allocate scalar arrays
    const uint32_t inputSpikePtr = AppUtils::allocateScalarAndZero(numInputSpikeWords * 4, scalarInitData);
    const uint32_t hiddenIsynScalarPtr = AppUtils::allocateScalarAndZero(64, scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);

    // Increase scalar memory for buffering
    scalarInitData.resize(128 * 1024, 0);
    
    // Load weights
    const auto weightInHid = AppUtils::loadBinaryData<uint8_t>("99-Conn_Pop0_Pop1-g.bin");
    const auto weightHidOut = AppUtils::loadBinaryData<uint8_t>("99-Conn_Pop1_Pop2-g.bin");
    const auto weightHidHid = AppUtils::loadBinaryData<uint8_t>("99-Conn_Pop1_Pop1-g.bin");
    const auto outputBias = AppUtils::loadBinaryData<uint8_t>("99-Pop2-Bias.bin");

    // Check first three are correctly padded
    assert(weightInHid.size() == numInput * numHiddenSpikeWords * 64);
    assert(weightHidOut.size() == numHidden * numOutputSpikeWords * 64);
    assert(weightHidHid.size() == numHidden * numHiddenSpikeWords * 64);
    
    // Concatenate into single vector
    std::vector<uint8_t> initData;
    initData.reserve(weightInHid.size() + weightHidOut.size() + weightHidHid.size() + outputBias.size());
    std::copy(weightInHid.cbegin(), weightInHid.cend(), std::back_inserter(initData));
    std::copy(weightHidOut.cbegin(), weightHidOut.cend(), std::back_inserter(initData));
    std::copy(weightHidHid.cbegin(), weightHidHid.cend(), std::back_inserter(initData));
    std::copy(outputBias.cbegin(), outputBias.cend(), std::back_inserter(initData));

    // Generate spikes
    /*{
        std::vector<uint32_t> test(numInputSpikeWords);
        for(size_t i = 0; i < numInputSpikeWords; i++) {
            test[i] = 0xDEADBEEF;
        }
        std::memcpy(scalarInitData.data() + inputSpikePtr, test.data(), test.size() * 4);
    }
    
    std::vector<uint32_t> wordData(scalarInitData.size() / 4);
    std::memcpy(wordData.data(), scalarInitData.data(), scalarInitData.size());
    AppUtils::dumpCOE("spike_data.coe", wordData);*/
    
     // Generate initialisation code to copy blocks of data from scalar to vector memory
    const uint32_t initStartVectorPtr = 0;
    const uint32_t initNumVectorsPtr = 4;
    const uint32_t initReadyFlagPtr = 8;
    const uint32_t initScalarScratchPtr = 12;
    const auto initCode = AssemblerUtils::generateInitCode(simulate, initStartVectorPtr, initNumVectorsPtr, 
                                                           initReadyFlagPtr, initScalarScratchPtr);
    
    // Generate sim code
    const auto simCode = generateSimCode(simulate, numInput, numHidden, numInputSpikeWords, numHiddenSpikeWords, 
                                         inputSpikePtr, weightInHidPtr, hiddenIsynPtr, hiddenIsynScalarPtr, readyFlagPtr);
    // Dump to coe file
    AppUtils::dumpCOE("spike.coe", simCode);
    LOGI << simCode.size() << " simulation instructions";
    LOGI << scalarInitData.size() << " bytes of scalar memory required";
    LOGI << vectorInitData.size() * 2 << " bytes of vector memory required (" << ceilDivide(vectorInitData.size() / 32, 4096) << " URAM cascade)";

    std::vector<uint32_t> spikes(numInputSpikeWords, 0);
    //const uint32_t spikes[4]{0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF, 0xDEADBEEF};
    spikes[0] = 1;
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
        
         // Copy input spike bits into scalar memory
         auto *scalarData = riscV.getScalarDataMemory().getData().data();
         std::copy(spikes.cbegin(), spikes.cend(),
                   reinterpret_cast<uint32_t*>(scalarData + inputSpikePtr));

        // Load simulation program
        riscV.setInstructions(simCode);
        riscV.setPC(0);
        if(!riscV.run()) {
            return 1;
        }
        
        
        const int16_t *hiddenIsyn = reinterpret_cast<const int16_t*>(scalarData + hiddenIsynScalarPtr);
        check(hiddenIsyn, numHidden);

    }
    else {
        LOGI << "Creating device";
        Device device;
        LOGI << "Resetting";
        // Put core into reset state
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
            
            // Copy input spike bits into scalar memory
            device.memcpyDataToDevice(inputSpikePtr, 
                                      reinterpret_cast<const uint8_t*>(spikes.data()),
                                      numInputSpikeWords);
            device.setILATrigger(true);
            device.setEnabled(true);
            
            // Wait until ready flag
            device.waitOnNonZero(readyFlagPtr);
            LOGI << "Done";
            device.setILATrigger(false);
            device.setEnabled(false);
            
            const volatile int16_t *hiddenIsyn = reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + hiddenIsynScalarPtr);
            check(hiddenIsyn, numHidden);
        }
        
        
    }
    return 0;
}
