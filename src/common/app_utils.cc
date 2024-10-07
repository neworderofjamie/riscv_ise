#include "common/app_utils.h"

// Standard C++ includes
#include <random>

// Plog includes
#include <plog/Log.h>

// RISC-V common includes
#include "common/utils.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/register_allocator.h"

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
void seedRNG(int16_t *seedPointer)
{
    // Fill first 64 half words of vector memory with random seed data
    std::random_device seedSource;
    for(size_t i = 0; i < 32; i++) {
        const uint32_t seed = seedSource();
        *seedPointer++ =  seed & 0xFFFF;
        *seedPointer++ = (seed >> 16) & 0xFFFF;
    }
}
}

//----------------------------------------------------------------------------
// AppUtils
//----------------------------------------------------------------------------
namespace AppUtils
{
uint32_t allocateVectorAndZero(size_t numHalfWords, std::vector<int16_t> &memory)
{
    // Ensure memory contents is aligned
    const size_t startHalfWords = memory.size();
    assert((startHalfWords % 32) == 0);

    const auto numVectors = ceilDivide(numHalfWords, 32);
    LOGD << "Allocating " << numHalfWords << " halfwords into " << numVectors << " vectors of memory starting at " << startHalfWords * 2 << " bytes";
    
    // Allocate memory and zero
    memory.resize(startHalfWords + (numVectors * 32), 0);

    // Return start address in bytes
    return static_cast<uint32_t>(startHalfWords) * 2;
}
//----------------------------------------------------------------------------
uint32_t allocateVectorSeedAndInit(std::vector<int16_t> &memory)
{
    // Allocate two vectors of seed
    const uint32_t startBytes = allocateVectorAndZero(64, memory);

    // Generate seed
    seedRNG(memory.data() + (startBytes / 2));
    
    return startBytes;
}
//----------------------------------------------------------------------------
uint32_t loadVectors(const std::string &filename, std::vector<int16_t> &memory)
{
    std::ifstream input(filename, std::ios::binary);
    
    // Get length
    input.seekg (0, std::ios::end);
    const auto lengthBytes = input.tellg();
    input.seekg (0, std::ios::beg);

    // Check contents is half-word aligned
    assert((lengthBytes % 2) == 0);

    // Allocate memory
    const uint32_t startBytes = allocateVectorAndZero(lengthBytes / 2, memory);

    // Read data directly into newly allocated memory
    input.read(reinterpret_cast<char*>(memory.data() + (startBytes / 2)), lengthBytes);

    // Return start address in bytes
    return startBytes;
}
//----------------------------------------------------------------------------
uint32_t allocateScalarSeedAndInit(std::vector<uint8_t> &memory)
{
    // Allocate two vectors of seed
    const uint32_t startBytes = allocateScalarAndZero(128, memory);

    // Generate seed
    seedRNG(reinterpret_cast<int16_t*>(memory.data() + startBytes));
    
    return startBytes;
}
//----------------------------------------------------------------------------
uint32_t allocateScalarAndZero(size_t numBytes, std::vector<uint8_t> &memory)
{
    const size_t startBytes = memory.size();
    assert((startBytes % 4) == 0);

    // Allocate memory and zero
    LOGD << "Allocating " << numBytes << " bytes of memory starting at " << startBytes << " bytes";
    memory.resize(startBytes + padSize(numBytes, 4), 0);

    // Return start address
    return static_cast<uint32_t>(startBytes);
}
//----------------------------------------------------------------------------
void writeSpikes(std::ofstream &os, const volatile uint32_t *data, 
                 float time, size_t numWords)
{
    // Loop through words
    for(size_t i = 0; i < numWords; i++) {
        // Read word
        uint32_t w = *data++;

        // While there are bits set in word
        unsigned int n = (i * 32) + 31;
        while(w != 0) {
            // Count leading zeros
            unsigned int numLZ = clz(w);

            // Shift off leading zeros
            w = (numLZ == 31) ? 0 : (w << (numLZ + 1));
            
            // Update index
            n -= numLZ;
            
            // Write CSV line
            os << time << ", " << n << std::endl;
            n--;

        }
    }
}
//----------------------------------------------------------------------------
void dumpCOE(const std::string &filename, const std::vector<uint32_t> &code)
{
    // Write hexadecimal COE file
    std::ofstream coe(filename);
    coe << "memory_initialization_radix = 16;" << std::endl;
    coe << "memory_initialization_vector = " << std::endl;
    for(size_t i = 0; i < code.size(); i++) {
        coe << std::hex << std::setfill('0') << std::setw(8) << code[i];
        if(i == (code.size() - 1)) {
            coe << ";";
        }
        else {
            coe << ",";
        }
        coe << std::endl;
    }
}
}
