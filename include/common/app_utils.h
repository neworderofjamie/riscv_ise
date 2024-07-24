#pragma once

// Standard C++ includes
#include <fstream>
#include <string>
#include <vector>

// Standard C includes
#include <cstdint>

//----------------------------------------------------------------------------
// AppUtils
//----------------------------------------------------------------------------
namespace AppUtils
{
// Allocate vector-aligned memory to store halfwords and return start adddress
uint32_t allocateVectorAndZero(size_t numHalfWords, std::vector<int16_t> &memory);

// Load vector data from int16_t binary file into vector-aligned memory
uint32_t loadVectors(const std::string &filename, std::vector<int16_t> &memory);

// Allocate word-aligned memory
uint32_t allocateScalarAndZero(size_t numBytes, std::vector<uint8_t> &memory);

// Write one timestep of spikes from a bitfield in CSV format
void writeSpikes(std::ofstream &os, const uint32_t *data, 
                 float time, size_t numWords);
}