#include "common/app_utils.h"

// Standard C++ includes
#include <random>

// Standard C includes
#include <cassert>

// Plog includes
#include <plog/Log.h>

// RISC-V common includes
#include "common/utils.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/register_allocator.h"

//----------------------------------------------------------------------------
// AppUtils
//----------------------------------------------------------------------------
namespace AppUtils
{
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
//----------------------------------------------------------------------------
uint32_t allocateVectorSeedAndInit(std::vector<int16_t> &memory)
{
    // Allocate two vectors of seed
    const uint32_t startBytes = allocateVectorAndZero(64, memory);

    // Fill first 64 half words of vector memory with random seed data
    std::random_device seedSource;
    int16_t *seedPointer = memory.data() + (startBytes / 2);
    for(size_t i = 0; i < 32; i++) {
        const uint32_t seed = seedSource();
        *seedPointer++ =  seed & 0xFFFF;
        *seedPointer++ = (seed >> 16) & 0xFFFF;
    }
    
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
    assert((lengthBytes & 1) == 0);

    // Allocate memory
    const uint32_t startBytes = allocateVectorAndZero(lengthBytes / 2, memory);

    // Read data directly into newly allocated memory
    input.read(reinterpret_cast<char*>(memory.data() + (startBytes / 2)), lengthBytes);

    // Return start address in bytes
    return startBytes;
}
//----------------------------------------------------------------------------
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
//----------------------------------------------------------------------------
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
//----------------------------------------------------------------------------
void generateScalarVectorMemCpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                ScalarRegisterAllocator &scalarRegisterAllocator,
                                uint32_t scalarPtr, uint32_t vectorPtr, uint32_t numVectors)
{
    // Register allocation
    ALLOCATE_SCALAR(SDataBuffer);
    ALLOCATE_SCALAR(SVectorBuffer)
    ALLOCATE_SCALAR(SThirtyTwo);
    ALLOCATE_SCALAR(SOne);
    ALLOCATE_SCALAR(SVectorBufferEnd);

    // Labels
    Label vectorLoop;
    Label laneLoop;

    // SThirtyTwo = 32
    c.li(*SThirtyTwo, 32);

    // SOne = 1
    c.li(*SOne, 1);

    c.li(*SVectorBuffer, vectorPtr);
    c.li(*SVectorBufferEnd, vectorPtr + (numVectors * 64));

    // SDataBuffer = scalarPtr
    c.li(*SDataBuffer, scalarPtr);

    // Loop over vectors
    c.L(vectorLoop);
    {
        // Register allocation
        ALLOCATE_VECTOR(VData);
        ALLOCATE_SCALAR(SLane);

        // SLane = 0
        c.li(*SLane, 0);

        c.L(laneLoop);
        {
            // Register allocation
            ALLOCATE_VECTOR(VLane);
            ALLOCATE_SCALAR(SMask);
            ALLOCATE_SCALAR(SVal);

            // Load halfword
            c.lh(*SVal, *SDataBuffer, 0);

            // Fill vector register
            c.vfill(*VLane, *SVal);

            // SDataBuffer += 2
            c.addi(*SDataBuffer, *SDataBuffer, 2);

            // SMask = 1 << SLane
            c.sll(*SMask, *SOne, *SLane);

            // VData = SMask ? VLane : VData
            c.vsel(*VData, *SMask, *VLane);

            // SLane++
            c.addi(*SLane, *SLane, 1);
            
            // If lane != 32, goto lane loop
            c.bne(*SLane, *SThirtyTwo, laneLoop);
        }
        
        // *SVectorBuffer = VData
        c.vstore(*VData, *SVectorBuffer);

        // SVector += 64
        c.addi(*SVectorBuffer, *SVectorBuffer, 64);

        // If SVectorBuffer != SVectorBufferEnd, goto vector loop
        c.bne(*SVectorBuffer, *SVectorBufferEnd, vectorLoop);
    }
}
}