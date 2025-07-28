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

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/assembler_utils.h"
#include "assembler/register_allocator.h"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"

void checkData(const std::vector<int16_t> &address, const volatile int16_t *scalarOutputData)
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
        std::cout << "Incorrect data:" << std::endl;
        for(int i = 0; i < 32; i++) {
            for(int j = 0; j < 32; j++) {
                std::cout << *scalarOutputData << ", ";
                scalarOutputData++;
            }
            std::cout << std::endl;
        }
    }
}
int main(int argc, char** argv)
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    bool device = false;
    bool dumpCoe = false;

    CLI::App app{"Lane-local memory test"};
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");
    app.add_flag("-c,--dump-coe", dumpCoe, "Should a .coe file for simulation in the Xilinx simulator be dumped");

    CLI11_PARSE(app, argc, argv);

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

    // Dump initial data to coe file
    if(dumpCoe) {
        std::vector<uint32_t> wordData(scalarInitData.size() / 4);
        std::memcpy(wordData.data(), scalarInitData.data(), scalarInitData.size());
        AppUtils::dumpCOE("lane_local_test_data.coe", wordData);
    }

    // Generate code
    const auto code = AssemblerUtils::generateStandardKernel(
        !device, readyFlagPtr,
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
        checkData(address, reinterpret_cast<const volatile int16_t*>(device.getDataMemory() + outputPtr));
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
        checkData(address, scalarOutputData);
    }
    LOGI << "Success!";
    
    return 0;
}
