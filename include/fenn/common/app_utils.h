#pragma once

// Standard C++ includes
#include <array>
#include <fstream>
#include <functional>
#include <optional>
#include <string>
#include <variant>
#include <vector>

// Standard C includes
#include <cassert>
#include <cstdint>

// Common includes
#include "fenn/common/fenn_common_export.h"

// Forward declations
namespace FeNN::Assembler
{
class CodeGenerator;
class ScalarRegisterAllocator;
class VectorRegisterAllocator;
}

//----------------------------------------------------------------------------
// AppUtils
//----------------------------------------------------------------------------
//! Helper functions for building applications with FeNN
namespace FeNN::Common::AppUtils
{
// Allocate vector-aligned memory to store halfwords and return start adddress
FENN_COMMON_EXPORT uint32_t allocateVectorAndZero(size_t numHalfWords, std::vector<int16_t> &memory);

// Seed RNG
FENN_COMMON_EXPORT uint32_t allocateVectorSeedAndInit(std::vector<int16_t> &memory,
                                                      const std::optional<std::array<int16_t, 64>> &seed = std::nullopt);

// Load vector data from int16_t binary file into vector-aligned memory
FENN_COMMON_EXPORT uint32_t loadVectors(const std::string &filename, std::vector<int16_t> &memory);

// Seed RNG
FENN_COMMON_EXPORT uint32_t allocateScalarSeedAndInit(std::vector<uint8_t> &memory,
                                                      const std::optional<std::array<int16_t, 64>> &seed = std::nullopt);

// Allocate word-aligned memory
FENN_COMMON_EXPORT uint32_t allocateScalarAndZero(size_t numBytes, std::vector<uint8_t> &memory);

FENN_COMMON_EXPORT uint32_t loadScalars(const std::string &filename, std::vector<uint8_t> &memory);

// Get seed data, in form suitable for bulk-copying
FENN_COMMON_EXPORT std::vector<uint8_t> getSeedData(const std::optional<std::array<int16_t, 64>> &seed = std::nullopt);

// Write one timestep of spikes from a bitfield in CSV format
FENN_COMMON_EXPORT void writeSpikes(std::ofstream &os, const volatile uint32_t *data,
                               float time, size_t numWords);

// Dump word-based data to a Vivado-format COE file
FENN_COMMON_EXPORT void dumpCOE(const std::string &filename, const std::vector<uint32_t> &code);

// Load binary file into vector
template<typename T>
std::vector<T> loadBinaryData(const std::string &filename)
{
    std::ifstream input(filename, std::ios::binary);
    input.exceptions(std::ifstream::badbit | std::ifstream::failbit);

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

}
