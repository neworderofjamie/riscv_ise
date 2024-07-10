// Standard C++ includes
#include <fstream>
#include <iterator>

// Standard C includes
#include <cassert>

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

//! Divide two integers, rounding up i.e. effectively taking ceil
template<typename A, typename B, typename = std::enable_if_t<std::is_integral_v<A> && std::is_integral_v<B>>>
inline auto ceilDivide(A numerator, B denominator)
{
    return ((numerator + denominator - 1) / denominator);
}

//! Pad an integer to a multiple of another
template<typename A, typename B, typename = std::enable_if_t<std::is_integral_v<A>&& std::is_integral_v<B>>>
inline auto padSize(A size, B blockSize)
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

int main()
{
    using namespace Xbyak_riscv;

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Allocate memory
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Constants
    const uint32_t numInput = 28 * 28;
    const uint32_t numHidden = 128;
    const uint32_t numOutput = 10;

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
    const uint32_t inputSpikeBuffer = allocateScalarAndZero(ceilDivide(numInput, 32) * 4, scalarInitData);
    const uint32_t hiddenSpikeBuffer = allocateScalarAndZero(ceilDivide(numHidden, 32) * 4, scalarInitData);

    CodeGenerator c;
    {
        // V0 = t = 0
        const auto VTime = VReg::V0;
        c.vlui(VTime, 4);

        // Loop over time
        Label timeLoop;
        c.L(timeLoop);
        {
            // **TODO** synapses
            
            // ---------------------------------------------------------------
            // Input neurons
            // ---------------------------------------------------------------
            {
                // SVecOffset = start of last vector of input
                const auto SVecOffset = Reg::X1;
                const uint32_t paddedNumInput = padSize(numInput, 32);
                c.li(SVecOffset, (paddedNumInput - 32) * 2);

                // SWordOffset = start of last word of input
                const auto SWordOffset = Reg::X2;
                c.li(SWordOffset, (ceilDivide(numInput, 32) - 1) * 4);
            
                // Calculate mask for first iteration
                const auto SMask = Reg::X3;
                c.li(SMask, (1 << (paddedNumInput - numInput)) - 1);

                // Input neuron loop
                Label inputNeuronLoop;
                c.L(inputNeuronLoop);
                {
                    // Load spike
                    //v1 = spikeTimeStart + vector offset
                    const auto VSpikeTime = VReg::V1;
                    c.vloadv(VSpikeTime, SVecOffset, spikeTimeStart);

                    // spike vector = x4 = spike time == t
                    const auto SSpikeVec = Reg::X4;
                    c.vseq(SSpikeVec, VTime, VSpikeTime);
                    c.and_(SSpikeVec, SSpikeVec, SMask);

                    // inputSpikeBuffer + scalarOffset = spike vector
                    c.sw(SSpikeVec, SWordOffset, inputSpikeBuffer);

                    // vector offset -= 64
                    c.addi(SVecOffset, SVecOffset, -64);

                    // scalar offset -= 4
                    c.addi(SWordOffset, SWordOffset, -4);

                    // reset mask
                    c.li(SMask, 0xFFFFFFFF);

                    // If scalar offset > 0, goto input loop
                    c.bge(SWordOffset, Reg::X0, inputNeuronLoop);
                }
            }

            // Hidden neuron loop
            Label hiddenNeuronLoop;
            c.L(hiddenNeuronLoop);
            {
                // **NOTE** mask not necessary as POT
            }

            // Output neuron loop
            Label inputNeuronLoop;
            c.L(inputNeuronLoop);
            {
                // **TODO** fixed mask
            }
        }

    }

    // Create RISC-V core with instruction and scalar data
    RISCV riscV(c.getCode(), scalarInitData);
    
    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
    // Run!
    riscV.run();
    
    // Decode spikes
    const auto &scalarData = riscV.getScalarDataMemory().getData();
    const uint32_t *inputSpikeWords = reinterpret_cast<const uint32_t*>(scalarData.data() + inputSpikeBuffer);
    for(size_t i = 0; i < 25; i++) {
        uint32_t w = inputSpikeWords[i];

        unsigned int n = (i * 32) + 31;
        while(w != 0) {
            unsigned int numLZ = clz(w);
            w = (numLZ == 31) ? 0 : (w << (numLZ + 1));
            n -= numLZ;
            
            std::cout << std::dec << n << " spiked!" << std::endl;
            n--;

        }
    }
    std::cout << std::endl;
}
