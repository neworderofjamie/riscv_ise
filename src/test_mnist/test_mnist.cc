// Standard C++ includes
#include <bitset>
#include <fstream>
#include <iterator>
#include <limits>
#include <memory>

// Standard C includes
#include <cassert>
#include <cmath>

// Platform includes
#ifdef _WIN32
#include <intrin.h>
#endif

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V assembler includes
#include "assembler/xbyak_riscv.hpp"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"

using namespace Xbyak_riscv;

#define ALLOCATE_SCALAR(NAME) const auto NAME = scalarRegisterAllocator.getRegister(#NAME" = X");
#define ALLOCATE_VECTOR(NAME) const auto NAME = vectorRegisterAllocator.getRegister(#NAME" = V");

int clz(uint32_t value)
{
#ifdef _WIN32
    unsigned long leadingZero = 0;
    if(_BitScanReverse(&leadingZero, value)) {
        return 31 - leadingZero;
    }
    else {
        return 32;
    }
#else
    return __builtin_clz(value);
#endif
}

template<typename T>
class RegisterAllocator
{
public:
    class Handle
    {
    public:
        Handle(T reg, RegisterAllocator<T> &parent, const char *context = nullptr)
        :   m_Reg(reg), m_Parent(parent)
        {
            if(context) {
                PLOGD << "Allocating " << context << static_cast<uint32_t>(reg);
            }
        }

        ~Handle()
        {
            m_Parent.releaseRegister(m_Reg);
        }
    
        operator T() const
        {
            return m_Reg;
        }

    private:
        T m_Reg;
        RegisterAllocator<T> &m_Parent;
    };

    RegisterAllocator(uint32_t initialFreeRegisters = 0xFFFFFFFFu)
    :   m_FreeRegisters(initialFreeRegisters)
    {
    }

    std::shared_ptr<Handle> getRegister(const char *context = nullptr)
    {
        if(m_FreeRegisters == 0) {
            throw std::runtime_error("Out of registers");
        }
        else {
            const int n = clz(m_FreeRegisters);
            m_FreeRegisters &= ~(0x80000000 >> n);
            return std::make_shared<Handle>(static_cast<T>(n), *this, context);
        }
    }
    
    void releaseRegister(T reg)
    {
        const uint32_t regNum = static_cast<uint32_t>(reg);

        if((m_FreeRegisters & (0x80000000 >> regNum)) != 0) {
            throw std::runtime_error("Releasing unused register");
        }
        else {
            m_FreeRegisters |= (0x80000000 >> regNum); 
        }
    }

private:
    uint32_t m_FreeRegisters;
};

//! Divide two integers, rounding up i.e. effectively taking ceil
template<typename A, typename B, typename = std::enable_if_t<std::is_integral_v<A> && std::is_integral_v<B>>>
constexpr inline auto ceilDivide(A numerator, B denominator)
{
    return ((numerator + denominator - 1) / denominator);
}

//! Pad an integer to a multiple of another
template<typename A, typename B, typename = std::enable_if_t<std::is_integral_v<A>&& std::is_integral_v<B>>>
constexpr inline auto padSize(A size, B blockSize)
{
    return ceilDivide(size, blockSize) * blockSize;
}


// Load vector data from int16_t binary file
uint32_t loadVectors(const std::string &filename, std::vector<int16_t> &memory)
{
    // Ensure memory contents is aligned
    const size_t startHalfWords = memory.size();
    assert((startHalfWords & 31) == 0);

    std::ifstream input(filename, std::ios::binary);
    
    // Get length
    input.seekg (0, std::ios::end);
    const auto lengthBytes = input.tellg();
    input.seekg (0, std::ios::beg);

    // Check contents is half-word aligned
    assert((lengthBytes & 1) == 0);

    const auto lengthHalfWords = lengthBytes / 2;
    const auto numVectors = ceilDivide(lengthHalfWords, 32);
    LOGD << "Loading " << lengthBytes << " bytes from " << filename << " into " << numVectors << " vectors of memory starting at " << startHalfWords * 2 << " bytes";
    
    // Allocate memory and initially zero
    memory.resize(startHalfWords + (numVectors * 32), 0);

    // Read data directly into it
    input.read(reinterpret_cast<char*>(memory.data() + startHalfWords), lengthBytes);

    // Return start address in bytes
    return static_cast<uint32_t>(startHalfWords) * 2;
}

uint32_t allocateVectorAndZero(size_t numHalfWords, std::vector<int16_t> &memory)
{
    // Ensure memory contents is aligned
    const size_t startHalfWords = memory.size();
    assert((startHalfWords & 31) == 0);

    const auto numVectors = ceilDivide(numHalfWords, 32);
    LOGD << "Allocating " << numHalfWords << " halfwords into " << numVectors << " vectors of memory starting at " << startHalfWords * 2 << " bytes";
    
    // Allocate memory and zero
    memory.resize(startHalfWords + (numVectors * 32), 0);

    // Return start address in bytes
    return static_cast<uint32_t>(startHalfWords) * 2;
}

uint32_t allocateScalarAndZero(size_t numBytes, std::vector<uint8_t> &memory)
{
    const size_t startBytes = memory.size();
    assert((startBytes & 3) == 0);

    // Allocate memory and zero
    LOGD << "Allocating " << numBytes << " bytes of memory starting at " << startBytes << " bytes";
    memory.resize(startBytes + padSize(numBytes, 4), 0);

    // Return start address
    return static_cast<uint32_t>(startBytes);
}

int16_t convertFixedPoint(double x, uint32_t fixedPoint)
{
    const double rounded = std::round(x * (1 << fixedPoint));
    assert(rounded >= std::numeric_limits<int16_t>::min());
    assert(rounded <= std::numeric_limits<int16_t>::max());

    return static_cast<int16_t>(rounded);
}

void writeSpikes(std::ofstream &os, const uint32_t *data, 
                 float time, size_t numWords)
{
    for(size_t i = 0; i < numWords; i++) {
        uint32_t w = *data++;;

        unsigned int n = (i * 32) + 31;
        while(w != 0) {
            unsigned int numLZ = clz(w);
            w = (numLZ == 31) ? 0 : (w << (numLZ + 1));
            n -= numLZ;
            
            os << time << ", " << n << std::endl;
            n--;

        }
    }
}

void genStaticPulse(CodeGenerator &c, RegisterAllocator<VReg> &vectorRegisterAllocator,
                    RegisterAllocator<Reg> &scalarRegisterAllocator,
                    uint32_t weightBuffer, uint32_t preSpikeBuffer, uint32_t postISynBuffer, 
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
    c.li(*SSpikeBuffer, preSpikeBuffer);
    
    // Get address of end of presynaptic spike buffer
    c.li(*SSpikeBufferEnd, ceilDivide(numPre, 32) * 4);
    c.add(*SSpikeBufferEnd, *SSpikeBufferEnd, *SSpikeBuffer);

    // SISynBuffer = hiddenIsyn;
    c.li(*SISynBuffer, postISynBuffer);

    {
        // Get size of postsynaptic vectors in bytes 
        // (ceildivide to number of vectors and multiply by 64 bytes per vector)
        ALLOCATE_SCALAR(STemp);
        c.li(*STemp, ceilDivide(numPost, 32) * 64);

        // SISynBufferEnd = SISynBuffer + SNumHiddenBytes
        c.add(*SISynBufferEnd, *SISynBuffer, *STemp);
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

            // SWeightBuffer = weightInHidStart + (numPostVecs * 64 * SN);
            // **TODO** multiply
            ALLOCATE_SCALAR(SWeightBuffer);
            c.li(*SWeightBuffer, weightBuffer);
            {
                ALLOCATE_SCALAR(STemp);
                c.slli(*STemp, *SN, scaleShift);
                c.add(*SWeightBuffer, *SWeightBuffer, *STemp);
            }

            // Reset Isyn pointer
            c.li(*SISynBuffer, postISynBuffer);
            
            // Load weight and Isyn
            if(debug) {
                c.ebreak();
            }

            // Loop over postsynaptic neurons
            if(numPost > 32) {
                // Input postsynaptic neuron loop
                c.L(weightLoop);
                {
                    ALLOCATE_VECTOR(VWeight);
                    ALLOCATE_VECTOR(VISyn);

                    // Load next vector of weights and ISyns
                    c.vloadv(*VWeight, *SWeightBuffer);
                    c.vloadv(*VISyn, *SISynBuffer);

                    // Add weights to ISyn
                    c.vadd(*VISyn, *VISyn, *VWeight);

                    // Write back ISyn
                    c.vstore(*VISyn, *SISynBuffer);

                    // Advance weights and Isyn
                    c.addi(*SWeightBuffer, *SWeightBuffer, 64);
                    c.addi(*SISynBuffer, *SISynBuffer, 64);

                    // If we haven't reached end of Isyn buffer, loop
                    c.bne(*SISynBuffer, *SISynBufferEnd, weightLoop);
                }
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

    // Load vector data **TODO** bias
    const uint32_t spikeTimeStart = loadVectors("mnist_7.bin", vectorInitData);
    const uint32_t weightInHidStart = loadVectors("mnist_in_hid.bin", vectorInitData);
    const uint32_t weightHidOutStart = loadVectors("mnist_hid_out.bin", vectorInitData);
    
    // Allocate additional vector arrays
    const uint32_t hiddenIsyn = allocateVectorAndZero(numHidden, vectorInitData);
    const uint32_t hiddenV = allocateVectorAndZero(numHidden, vectorInitData);

    const uint32_t outputIsyn = allocateVectorAndZero(numOutput, vectorInitData);
    const uint32_t outputV = allocateVectorAndZero(numOutput, vectorInitData);
    const uint32_t outputVSum = allocateVectorAndZero(numOutput, vectorInitData);
   
    // Allocate scalar arrays
    const uint32_t timestep = allocateScalarAndZero(4, scalarInitData);
    const uint32_t inputSpikeBuffer = allocateScalarAndZero(numInputSpikeWords * 4, scalarInitData);
    const uint32_t hiddenSpikeBuffer = allocateScalarAndZero(numHiddenSpikeWords * 4, scalarInitData);

    CodeGenerator c;
    {
        RegisterAllocator<VReg> vectorRegisterAllocator;
        RegisterAllocator<Reg> scalarRegisterAllocator(0x7FFFFFFFu);

        // V0 = *timestep
        ALLOCATE_VECTOR(VTime);
        {
            ALLOCATE_SCALAR(STemp);
            c.li(*STemp, timestep);
            c.vloads(*VTime, *STemp);
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
                           weightInHidStart, inputSpikeBuffer, 
                           hiddenIsyn, numInput, numHidden, 8, false);

            // ---------------------------------------------------------------
            // Hidden->Output synapses
            // ---------------------------------------------------------------
            // 2^6 = 2 bytes * 32 output neurons (rounded up)
            genStaticPulse(c, vectorRegisterAllocator, scalarRegisterAllocator, 
                           weightHidOutStart, hiddenSpikeBuffer, 
                           outputIsyn, numHidden, numOutput, 6, false);

            // ---------------------------------------------------------------
            // Input neurons
            // ---------------------------------------------------------------
            {
                // Register allocation
                ALLOCATE_SCALAR(SSpikeBuffer);
                ALLOCATE_SCALAR(SSpikeBufferEnd);
                ALLOCATE_SCALAR(SSpikeTimeBuffer);
                ALLOCATE_SCALAR(SNumSpikeBytes);

                // Labels
                Label neuronLoop;

                // Get address of spike and spike time buffer
                c.li(*SSpikeBuffer, inputSpikeBuffer);
                c.li(*SSpikeTimeBuffer, spikeTimeStart);

                // SNumHiddenBytes = number of bytes of full vector
                c.li(*SNumSpikeBytes, (numInput / 32) * 4);

                // SSpikeBufferEnd = SSpikeBufer + SNumSpikeBytes
                c.add(*SSpikeBufferEnd, *SSpikeBuffer, *SNumSpikeBytes);

                // Input neuron loop
                c.L(neuronLoop);
                {
                    // Register allocation
                    ALLOCATE_VECTOR(VSpikeTime);
                    ALLOCATE_SCALAR(SSpikeVec);

                    // Load spike
                    c.vloadv(*VSpikeTime, *SSpikeTimeBuffer);

                    // spike vector = x4 = spike time == t
                    c.vteq(*SSpikeVec, *VTime, *VSpikeTime);

                    // inputSpikeBuffer + scalarOffset = spike vector
                    c.sw(*SSpikeVec, *SSpikeBuffer);

                    // SSpikeTimeBuffer += 64
                    c.addi(*SSpikeTimeBuffer, *SSpikeTimeBuffer, 64);

                    // SSpikeBuffe += 4
                    c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);

                    // If SSpikeBuffer != SSpikeBufferEnd, goto input loop
                    c.bne(*SSpikeBuffer, *SSpikeBufferEnd, neuronLoop);
                }

                // Input neuron tail
                {
                    // Register allocation
                    ALLOCATE_VECTOR(VSpikeTime);
                    ALLOCATE_SCALAR(SMask);
                    ALLOCATE_SCALAR(SSpikeVec);
                    
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

            // ---------------------------------------------------------------
            // Hidden neurons
            // ---------------------------------------------------------------
            {
                // Register allocation
                ALLOCATE_SCALAR(SVBuffer);
                ALLOCATE_SCALAR(SISynBuffer);
                ALLOCATE_SCALAR(SSpikeBuffer);
                ALLOCATE_SCALAR(SSpikeBufferEnd);
                ALLOCATE_SCALAR(SNumSpikeBytes);
                ALLOCATE_VECTOR(VAlpha);
                ALLOCATE_VECTOR(VThresh);
                ALLOCATE_VECTOR(VReset);
                
                // Labels
                Label neuronLoop;
                
                // Load constants
                // alpha = e^(-1/20)
                c.vlui(*VAlpha, convertFixedPoint(std::exp(-1.0 / 20.0), hiddenFixedPoint));
                
                // v = 0
                c.vlui(*VReset, 0);
                
                // v_thresh = 1
                c.vlui(*VThresh, convertFixedPoint(0.61, hiddenFixedPoint));

                // Get address of spike and spike time buffer
                c.li(*SVBuffer, hiddenV);
                c.li(*SISynBuffer, hiddenIsyn);
                c.li(*SSpikeBuffer, hiddenSpikeBuffer);

                // SNumSpikeBytes = numHidden * 4
                c.li(*SNumSpikeBytes, numHiddenSpikeWords * 4);

                // SSpikeBufferEnd = SSpikeBufer + SNumSpikeBytes
                c.add(*SSpikeBufferEnd, *SSpikeBuffer, *SNumSpikeBytes);

                // Input neuron loop
                c.L(neuronLoop);
                {
                    // Register allocation
                    ALLOCATE_VECTOR(VV);
                    ALLOCATE_VECTOR(VISyn);
                    ALLOCATE_SCALAR(SSpikeOut);

                    // Load voltage and isyn
                    c.vloadv(*VV, *SVBuffer);
                    c.vloadv(*VISyn, *SISynBuffer);

                    // VV *= VAlpha
                    c.vmul(hiddenFixedPoint, *VV, *VV, *VAlpha);

                    // VV += VISyn
                    c.vadd(*VV, *VV, *VISyn);

                    // VISyn = 0
                    c.vlui(*VISyn, 0);

                    // SSpikeOut = VV >= VThresh
                    c.vtge(*SSpikeOut, *VV, *VThresh);

                    // *SSpikeBuffer = SSpikeOut
                    c.sw(*SSpikeOut, *SSpikeBuffer);

                    // VV = SSpikeOut ? VReset : VV
                    c.vsel(*VV, *SSpikeOut, *VReset);

                    // Store VV and ISyn
                    c.vstore(*VV, *SVBuffer);
                    c.vstore(*VISyn, *SISynBuffer);

                    // SVBuffer += 64
                    c.addi(*SVBuffer, *SVBuffer, 64);
                    c.addi(*SISynBuffer, *SISynBuffer, 64);
                    c.addi(*SSpikeBuffer, *SSpikeBuffer, 4);

                    // If SSpikeBuffer != SSpikeBufferEnd, loop
                    c.bne(*SSpikeBuffer, *SSpikeBufferEnd, neuronLoop);
                }
            }

            // End
            c.ecall();
        }

    }

    // Create RISC-V core with instruction and scalar data
    RISCV riscV(c.getCode(), scalarInitData);
    
    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
    // Recording data
    std::vector<uint32_t> inputSpikeRecording;
    std::vector<uint32_t> hiddenSpikeRecording;
    inputSpikeRecording.reserve(79 * numInputSpikeWords);
    hiddenSpikeRecording.reserve(79 * numHiddenSpikeWords);

    // Loop through time
    auto *scalarData = riscV.getScalarDataMemory().getData().data();
    const uint32_t *inputSpikeWords = reinterpret_cast<const uint32_t*>(scalarData + inputSpikeBuffer);
    const uint32_t *hiddenSpikeWords = reinterpret_cast<const uint32_t*>(scalarData + hiddenSpikeBuffer);
    for(uint32_t t = 0; t < 79; t++) {
        // Copy timestep into scalar memory
        std::memcpy(scalarData + timestep, &t, 4);

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
    }

    std::ofstream inputSpikes("input_spikes.csv");
    std::ofstream hiddenSpikes("hidden_spikes.csv");
    for(uint32_t t = 0; t < 79; t++) {
        writeSpikes(inputSpikes, inputSpikeRecording.data() + (numInputSpikeWords * t),
                    t, numInputSpikeWords);
        writeSpikes(hiddenSpikes, hiddenSpikeRecording.data() + (numHiddenSpikeWords * t),
                    t, numHiddenSpikeWords);
    }

    return 0;
}
