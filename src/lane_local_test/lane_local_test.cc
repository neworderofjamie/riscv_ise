// Standard C++ includes
#include <fstream>
#include <numeric>

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

    // Generate address vector and copy into scalar memory
    std::vector<int16_t> address{0, -1, 4, -1, 8, -1, 12, -1, 16, -1, 20, -1, 24, -1, 28, -1, 32,
                                 -1, 36, -1, 40, -1, 44, -1, 48, -1, 52, -1, 56, -1, 60, -1};
    assert(address.size() == 32);
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

            {
                ALLOCATE_SCALAR(SOutputBuffer);
                ALLOCATE_SCALAR(SOutputBufferEnd);
                ALLOCATE_VECTOR(VAddress);
                ALLOCATE_VECTOR(VTwo);

                c.vlui(*VAddress, 0);
                c.vlui(*VTwo, 2);

                // SDataBuffer = scalarPtr
                c.li(*SOutputBuffer, outputPtr);
                c.li(*SOutputBufferEnd, outputPtr + (64 * 32));

                // Loop over time
                c.L(loop);
                {
                    // Register allocation
                    ALLOCATE_VECTOR(VData);
                    ALLOCATE_SCALAR(SVal);

                    // Load from lane-local memory into vector and increment address
                    c.vloadl(*VData, *VAddress, 0);
                    c.vadd(*VAddress, *VAddress, *VTwo);

                    // Extract lanes and write to scalar memory
                    for(int l = 0; l < 32; l++) {
                        // Extract lane into scalar registers
                        c.vextract(*SVal, *VData, l);

                        // Store halfword
                        c.sh(*SVal, *SOutputBuffer, l * 2);
                    }

                    // SOutputBuffer += 64
                    c.addi(*SOutputBuffer, *SOutputBuffer, 64);

                    // If SOutputBuffer != SOutputBufferEnd, goto vector loop
                    c.bne(*SOutputBuffer, *SOutputBufferEnd, loop);
                }
            }

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
   
    
    return 0;
}
