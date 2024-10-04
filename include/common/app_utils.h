#pragma once

// Standard C++ includes
#include <fstream>
#include <functional>
#include <string>
#include <vector>

// Standard C includes
#include <cstdint>

// Common includes
#include "common/isa.h"

// Forward declations
class CodeGenerator;
class ScalarRegisterAllocator;
class VectorRegisterAllocator;

//----------------------------------------------------------------------------
// AppUtils
//----------------------------------------------------------------------------
namespace AppUtils
{
// Allocate vector-aligned memory to store halfwords and return start adddress
uint32_t allocateVectorAndZero(size_t numHalfWords, std::vector<int16_t> &memory);

// Seed RNG
uint32_t allocateVectorSeedAndInit(std::vector<int16_t> &memory);

// Load vector data from int16_t binary file into vector-aligned memory
uint32_t loadVectors(const std::string &filename, std::vector<int16_t> &memory);

// Allocate word-aligned memory
uint32_t allocateScalarAndZero(size_t numBytes, std::vector<uint8_t> &memory);

// Write one timestep of spikes from a bitfield in CSV format
void writeSpikes(std::ofstream &os, const uint32_t *data, 
                 float time, size_t numWords);

// Dump word-based data to a Vivado-format COE file
void dumpCOE(const std::string &filename, const std::vector<uint32_t> &code);

// Generate
void generateScalarVectorMemcpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                ScalarRegisterAllocator &scalarRegisterAllocator,
                                uint32_t scalarPtr, uint32_t vectorPtr, uint32_t numVectors);

void generateVectorScalarMemcpy(CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator,
                                ScalarRegisterAllocator &scalarRegisterAllocator,
                                uint32_t vectorPtr, uint32_t scalarPtr, uint32_t numVectors);

// Generate an unrolled loop body
void unrollLoopBody(CodeGenerator &c, uint32_t numIterations, uint32_t maxUnroll, 
                    Reg testBufferReg, Reg testBufferEndReg, 
                    std::function<void(CodeGenerator&, uint32_t)> genBodyFn, 
                    std::function<void(CodeGenerator&, uint32_t)> genTailFn);
}
