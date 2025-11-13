// Standard C++ includes
#include <fstream>
#include <numeric>
#include <random>

// PLOG includes
#include <plog/Severity.h>

// Google test includes
#include "gtest/gtest.h"

// RISC-V common includes
#include "common/app_utils.h"
#include "common/device.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"

// SoC test includes
#include "fixtures.h"

// Anonymous namespace
namespace
{
bool checkLaneLocalData(const std::vector<int16_t> &address, const volatile int16_t *scalarOutputData)
{
    // Loop through COLUMNS of data
    bool correct = true;
    for(int j = 0; j < 32; j++) {
        // Get row where three should be located in this column
        const int16_t threeLoc = address[j];
        
        // Loop through rows
        for(int i = 0; i < 32; i++) {
            const int16_t val = scalarOutputData[(i * 32) + j];
       
            if(threeLoc >= 0 && i == (threeLoc / 2)) {
                if(val != 3) {
                    correct = false;
                }
            }
            else if(val != 1) {
                correct = false;
            }
        }
    }

    if(!correct) {
        LOGE << "Incorrect data:";
        for(int i = 0; i < 32; i++) {
            for(int j = 0; j < 32; j++) {
                std::cout << *scalarOutputData << ", ";
                scalarOutputData++;
            }
            std::cout << std::endl;
        }
    }
    
    return correct;
}

bool checkScalarData(const volatile int16_t *scalarInputData, 
                     const volatile int16_t *scalarOutputData)
{
    // Verify copy was successful
    bool correct = true;
    for(size_t i = 0; i < (32 * 10); i++) {
        const int16_t read = scalarInputData[i];
        const int16_t write = scalarOutputData[i];
        std::cout << read << "(" << write << "), ";
        if(read != write) {
            correct = false;
        }
    }
    std::cout << std::endl;
    return correct;
}
}

//--------------------------------------------------------------------------
// Tests
//--------------------------------------------------------------------------
TEST_P(DeviceSimTest, LaneLocal)
{
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate vector arrays
    const uint32_t addressVectorPtr = AppUtils::allocateVectorAndZero(32, vectorInitData);

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t addressPtr = AppUtils::allocateScalarAndZero(64, scalarInitData);
    const uint32_t outputPtr = AppUtils::allocateScalarAndZero(64 * 32, scalarInitData);

    // Generate random address vector
    std::vector<int16_t> address(32);
    std::random_device r;
    std::uniform_int_distribution<int16_t> dist(-32, 32);
    for(int i = 0; i < 32; i++) {
        const int16_t val = dist(r);
        if(val < 0) {
            address[i] = -1;
        }
        else {
            address[i] = val * 2;
        }
        std::cout << address[i] << ", ";
    }
    std::cout << std::endl;
    
    // Copy into scalar memory
    std::memcpy(scalarInitData.data() + addressPtr, address.data(), address.size() * 2);

    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !GetParam(), readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Generate code to copy address vector from scalar memory to vector memory
            AssemblerUtils::generateScalarVectorMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       addressPtr, addressVectorPtr, 1u);

            // Labels
            Label loop;

            {
                // Register allocation
                ALLOCATE_VECTOR(VZero);
                ALLOCATE_VECTOR(VOne);
                
                c.vlui(*VZero, 0);
                c.vlui(*VOne, 1);

                // Set first 32 addresses of lane local memory to 1
                for(int i = 0; i < 32; i++) {
                    c.vstorel(*VOne, *VZero, i * 2);
                }
            }
            
            {
                // Register allocation
                ALLOCATE_SCALAR(SAddressBuffer);
                ALLOCATE_VECTOR(VAddress);
                ALLOCATE_VECTOR(VTwo);
                ALLOCATE_VECTOR(VDest);

                // Load addresses
                c.li(*SAddressBuffer, addressVectorPtr);
                c.vloadv(*VAddress, *SAddressBuffer);

                // Load 2
                c.vlui(*VTwo, 2);

                // Load from lane-local addresses into vector (should be 1)
                c.vloadl(*VDest, *VAddress);

                // Stall
                c.nop();

                // Add two
                c.vadd(*VDest, *VDest, *VTwo);

                // Store back to lane local memory
                c.vstorel(*VDest, *VAddress);
            }
            
            // Copy lane-local data to output pointer
            AssemblerUtils::generateLaneLocalScalarMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                          0, outputPtr, 32);

        });

    if(GetParam()) {
        LOGI << "Creating device";
        Device device;
        LOGI << "Resetting";
        // Put core into reset state
        device.setEnabled(false);
        
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

        // Check data
        EXPECT_TRUE(checkLaneLocalData(address, reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + outputPtr)));
    }
    else {
        // Build ISE with vector co-processor
        RISCV riscV;
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);

        // Set instructions and vector init data
        riscV.setInstructions(code);
        riscV.getScalarDataMemory().setData(scalarInitData);

        // Run!
        riscV.run();
        
        // Get pointers to output data in scalar memory and validate
        const auto *scalarData = riscV.getScalarDataMemory().getData();
        const int16_t *scalarOutputData = reinterpret_cast<const int16_t*>(scalarData + outputPtr);
        EXPECT_TRUE(checkLaneLocalData(address, scalarOutputData));
    }
}

TEST_P(DeviceSimTest, URAM)
{
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate vector arrays
    const uint32_t vectorPtr = AppUtils::allocateVectorAndZero(32 * 10, vectorInitData);

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t inputPtr = AppUtils::allocateScalarAndZero(32 * 10 * 2, scalarInitData);
    const uint32_t outputPtr = AppUtils::allocateScalarAndZero(32 * 10 * 2, scalarInitData);    
    
    // Generate 10 vectors of increasing numbers and copy into scalar memory
    std::vector<int16_t> test(10 * 32);
    std::iota(test.begin(), test.end(), 0);
    std::memcpy(scalarInitData.data() + inputPtr, test.data(), 10 * 32 * 2);

    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !GetParam(), readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Generate memcpy from inputPtr to vectorPtr
            AssemblerUtils::generateScalarVectorMemcpy(c, vectorRegisterAllocator,
                                                       scalarRegisterAllocator,
                                                       inputPtr, vectorPtr, 10u);

            // Generate memcpy back from vectorPtr to outputPtr
            AssemblerUtils::generateVectorScalarMemcpy(c, vectorRegisterAllocator,
                                                       scalarRegisterAllocator,
                                                       vectorPtr, outputPtr, 10);
        });
    
    if(GetParam()) {
        LOGI << "Creating device";
        Device device;
        LOGI << "Resetting";
        // Put core into reset state
        device.setEnabled(false);
        
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

        // Check data
        EXPECT_TRUE(checkScalarData(reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + inputPtr),
                                    reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + outputPtr)));
    }
    else {
        // Build ISE with vector co-processor
        RISCV riscV;
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant);

        // Set instructions and vector init data
        riscV.setInstructions(code);
        riscV.getScalarDataMemory().setData(scalarInitData);

        // Run!
        riscV.run();
        
        // Get pointers to output data in scalar memory and validate
        const auto *scalarData = riscV.getScalarDataMemory().getData();
        EXPECT_TRUE(checkScalarData(reinterpret_cast<const int16_t*>(scalarData + inputPtr),
                                    reinterpret_cast<const int16_t*>(scalarData + outputPtr)));
    }
}

INSTANTIATE_TEST_SUITE_P(Memory,
                         DeviceSimTest,
                         testing::Values(true, false));

