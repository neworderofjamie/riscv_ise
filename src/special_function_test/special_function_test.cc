// Standard C++ includes
#include <fstream>
#include <numeric>
#include <random>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V common includes
#include "common/CLI11.hpp"
#include "common/app_utils.h"
#include "common/device.h"
#include "common/dma_buffer.h"
#include "common/dma_controller.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"

void writeData(const volatile int16_t *scalarOutputData, size_t numTestVectors)
{
    std::ofstream output("special_function_output.csv");
    
    for(uint32_t n = 0; n < (32 * numTestVectors); n++) {
        output << *scalarOutputData++ << std::endl;
    }
}

int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    bool device = false;
    bool dumpCoe = false;

    CLI::App app{"Special function test"};
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");
    app.add_flag("-c,--dump-coe", dumpCoe, "Should a .coe file for simulation in the Xilinx simulator be dumped");

    CLI11_PARSE(app, argc, argv);

      // Build LUT
    constexpr size_t numBits = 15;
    constexpr size_t tableBits = (numBits - 3) / 2;
    constexpr size_t fracBits = tableBits + 3;
    constexpr size_t lutSize = (1 << tableBits) + 1;
    constexpr size_t valueFixedPoint = 9;
    const double min = -3.4;
    const double max = 3.4;
    const int16_t minFixed = convertFixedPoint(min, valueFixedPoint);
    const int16_t maxFixed = convertFixedPoint(max, valueFixedPoint);

    const size_t count = maxFixed - minFixed;
    const size_t numTestVectors = ceilDivide(count, 32);

    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate vector arrays
    const uint32_t inputDataPtr = AppUtils::allocateVectorAndZero(32 * numTestVectors, vectorInitData);
    const uint32_t outputDataPtr = AppUtils::allocateVectorAndZero(32 * numTestVectors, vectorInitData);

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t outputScalarDataPtr = AppUtils::allocateScalarAndZero(32 * numTestVectors * 2, scalarInitData);
    const uint32_t lutScalarDataPtr = AppUtils::allocateScalarAndZero(65 * 2, scalarInitData);
    
  

    // Build LUT
    std::vector<int16_t> lut;
    lut.reserve(lutSize);
    const double log2 = std::log(2.0);
    const double expMax = 0.5 * log2;
    const double step = (2.0 * expMax) / (1 << tableBits);

    for(double x = -expMax; x < expMax; x += step) {
        lut.push_back(convertFixedPoint(std::exp(x), 14));
    }
    assert(lut.size() == lutSize);
    
    // Copy into scalar memory
    std::memcpy(scalarInitData.data() + lutScalarDataPtr, lut.data(), lut.size() * 2);

    // Write test vector to vector memory
    std::iota(vectorInitData.begin() + (inputDataPtr / 2),
              vectorInitData.begin() + (inputDataPtr / 2) + (32 * numTestVectors), minFixed);

    // Dump initial data to coe file
    if(dumpCoe) {
        std::vector<uint32_t> wordData(scalarInitData.size() / 4);
        std::memcpy(wordData.data(), scalarInitData.data(), scalarInitData.size());
        AppUtils::dumpCOE("special_function_test_data.coe", wordData);
    }

    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Generate code to broadcast LUT to start of all lane-local memories
            AssemblerUtils::generateScalarLaneLocalBroadcast(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                             lutScalarDataPtr, 0, lutSize);
            // Register allocation
            ALLOCATE_SCALAR(SInputBuffer);
            ALLOCATE_SCALAR(SInputBufferEnd);
            ALLOCATE_SCALAR(SOutputBuffer);
            ALLOCATE_VECTOR(VShiftScale);
            ALLOCATE_VECTOR(VFracMask);
            ALLOCATE_VECTOR(VInvLog);
            ALLOCATE_VECTOR(VLog2);
            ALLOCATE_VECTOR(VExpMax);
            ALLOCATE_VECTOR(VExpMaxScale);

            // Labels
            Label vectorLoop;

            // Load addresses
            c.li(*SInputBuffer, inputDataPtr);
            c.li(*SInputBufferEnd, inputDataPtr + (32 * numTestVectors * 2));
            c.li(*SOutputBuffer, outputDataPtr);

            // Load constants
            c.vlui(*VShiftScale, 14 - valueFixedPoint);
            c.vlui(*VFracMask, (1 << fracBits) - 1);
            c.vlui(*VLog2, convertFixedPoint(log2, 15));
            c.vlui(*VInvLog, convertFixedPoint(1.0 / log2, 14));
            c.vlui(*VExpMax, convertFixedPoint(expMax, valueFixedPoint));
            c.vlui(*VExpMaxScale, convertFixedPoint(1.0 / (2.0 * expMax), 14));

            // Loop over vectors
            c.L(vectorLoop);
            {
                ALLOCATE_VECTOR(VLUTAddress);
                ALLOCATE_VECTOR(VLUTLower);
                ALLOCATE_VECTOR(VLUTDiff);
                ALLOCATE_VECTOR(VX);
                ALLOCATE_VECTOR(VK);
                ALLOCATE_VECTOR(VR);
                ALLOCATE_VECTOR(VOutput);

                // Load input and increment buffer
                c.vloadv(*VX, *SInputBuffer);
                c.addi(*SInputBuffer, *SInputBuffer, 64);       
                
                // START RANGE-REDUCTION
                // VK = floor((VX * VInvLog) + 0.5).
                c.vmul(14, *VK, *VX, *VInvLog);
                c.vsrai_rn(valueFixedPoint, *VK, *VK);

                // VR = VX - (VK * VLog2)
                c.vmul_rn(15 - valueFixedPoint, *VR, *VK, *VLog2);
                c.vsub(*VR, *VX, *VR);

                // VR = (VR - VExpMax) / (VExpMax - -VExpMax)
                c.vadd(*VR, *VR, *VExpMax);
                c.vmul_rn(valueFixedPoint - 1, *VR, *VR, *VExpMaxScale);

                // START FAITHFUL INTERPOLATION
                // VLUTAddress = VX >> fracBits
                c.vsrai(fracBits, *VLUTAddress, *VR);

                // VLUTAddress *= 2 (to convert to bytes)
                c.vslli(1, *VLUTAddress, *VLUTAddress);

                // Load lower LUT entry
                c.vloadl(*VLUTLower, *VLUTAddress, 0);

                // Load higher LUT value
                c.vloadl(*VLUTDiff, *VLUTAddress, 2);

                // VOutput = VX & VFracMask
                c.vand(*VOutput, *VR, *VFracMask);

                // Calculate difference
                c.vsub(*VLUTDiff, *VLUTDiff, *VLUTLower);
           
                // VOutput *= 
                c.vmul_rn(fracBits, *VOutput, *VOutput, *VLUTDiff);

                c.vadd(*VOutput, *VOutput, *VLUTLower);

                // K = shiftScale - K to include shift to 
                // convert from S1.14 to output forma
                c.vsub(*VK, *VShiftScale, *VK);

                // END FAITHFUL INTERPOLATION
                c.vsra(*VOutput, *VOutput, *VK);

                // END RANGE-REDUCTION
                // Write to output buffer
                c.vstore(*VOutput, *SOutputBuffer);
                c.addi(*SOutputBuffer, *SOutputBuffer, 64);

                // Loop
                c.bne(*SInputBuffer, *SInputBufferEnd, vectorLoop);
            }


            // Copy output vector memory to BRAM
            AssemblerUtils::generateVectorScalarMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       outputDataPtr, outputScalarDataPtr, numTestVectors);

        });

    if(dumpCoe) {
        AppUtils::dumpCOE("lane_local_test.coe", code);
        return 0;
    }

    if(device) {
        LOGI << "Creating device";
        Device device;
        LOGI << "Resetting";
        // Put core into reset state
        device.setEnabled(false);

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
        
        LOGI << "Copying instructions (" << code.size() * sizeof(uint32_t) << " bytes)";
        device.uploadCode(code);
        
        LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
        device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());
        
        LOGI << "Enabling";
        // Put core into running state
        device.setEnabled(true);
        LOGI << "Running";
        
        // Wait until ready flag
        device.waitOnNonZero(readyFlagPtr);
        device.setEnabled(false);
        LOGI << "Done";

        // Write data to text file
        writeData(reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + outputScalarDataPtr), numTestVectors);
    }
    else {
        // Build ISE with vector co-processor
        RISCV riscV;
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);
        riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().setData(vectorInitData);

        // Set instructions and vector init data
        riscV.setInstructions(code);
        riscV.getScalarDataMemory().setData(scalarInitData);

        // Run!
        riscV.run();
        
        // Write data to text file
        const auto *scalarData = riscV.getScalarDataMemory().getData();
        const int16_t *scalarOutputData = reinterpret_cast<const int16_t*>(scalarData + outputScalarDataPtr);
        writeData(scalarOutputData, numTestVectors);
    }
    LOGI << "Success!";
    
    return 0;
}
