// Standard C++ includes
#include <fstream>
#include <random>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

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

std::vector<uint32_t> generateCode(bool simulate, unsigned int numVectorMultiply,
                                   uint32_t seedPtr, uint32_t scalarSeedPtr, uint32_t scalarOperandPtr, uint32_t readyFlagPtr)
{
   return AssemblerUtils::generateStandardKernel(
        simulate, readyFlagPtr,
        [=](CodeGenerator &c, VectorRegisterAllocator &vectorRegisterAllocator, ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            // Copy seed from scalar to vector memory
            AssemblerUtils::generateScalarVectorMemcpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                                       scalarSeedPtr, seedPtr, 2u);
            // Register allocation
            ALLOCATE_SCALAR(SBuffer);
            ALLOCATE_SCALAR(SBufferEnd);
            ALLOCATE_SCALAR(SOne);

            // Labels
            Label mulLoop;

            // Load RNG seed
            {
                ALLOCATE_SCALAR(STmp);
                c.li(*STmp, seedPtr);
                c.vloadr0(*STmp);
                c.vloadr1(*STmp, 64);
            }
            
            // Load immedate
            c.li(*SOne, 1);
            c.li(*SBuffer, scalarOperandPtr);
            c.li(*SBufferEnd, scalarOperandPtr + (3 * 64 * numVectorMultiply));

            // Loop over time
            c.L(mulLoop);
            {
                // Register allocation
                ALLOCATE_VECTOR(VA);
                ALLOCATE_VECTOR(VB);
                ALLOCATE_VECTOR(VRz);
                ALLOCATE_VECTOR(VRn);
                ALLOCATE_VECTOR(VRs);
                
                // Load operands
                {
                    ALLOCATE_VECTOR(VLane);
                    ALLOCATE_SCALAR(SMask);
                    ALLOCATE_SCALAR(SVal);
                    for(int i = 0; i < 32; i++) {
                        // SMask = 1 << SLane
                        c.slli(*SMask, *SOne, i);

                        // Load A halfword
                        c.lh(*SVal, *SBuffer, i * 2);

                        // Fill vector register
                        c.vfill(*VLane, *SVal);

                        // VA = SMask ? VLane : VData
                        c.vsel(*VA, *SMask, *VLane); 

                        // Load B halfword
                        c.lh(*SVal, *SBuffer, 64 + (i * 2));

                        // Fill vector register
                        c.vfill(*VLane, *SVal);

                        // VA = SMask ? VLane : VData
                        c.vsel(*VB, *SMask, *VLane); 
                    }
                }

                // Multiply operands using each instruction
                c.vmul(15, *VRz, *VA, *VB);
                c.vmul_rn(15, *VRn, *VA, *VB);
                c.vmul_rs(15, *VRs, *VA, *VB);
        
                // Store results
                {
                    ALLOCATE_SCALAR(SVal);
                    for(int i = 0; i < 32; i++) {
                        // Extract VRz into scalar registers and store
                        c.vextract(*SVal, *VRz, i);
                        c.sh(*SVal, *SBuffer, i * 2);

                        // Extract VRn into scalar registers and store
                        c.vextract(*SVal, *VRn, i);
                        c.sh(*SVal, *SBuffer, (i * 2) + 64);

                        // Extract VRz into scalar registers and store
                        c.vextract(*SVal, *VRs, i);
                        c.sh(*SVal, *SBuffer, (i * 2) + 128);
                    }
                }
                
                c.addi(*SBuffer, *SBuffer, 64 * 3);

                // While x2 (address) < x1 (count), goto loop
                c.bne(*SBuffer, *SBufferEnd, mulLoop);
            }
        });
}


int main()
{
    constexpr unsigned int numVectorMultiply = 680;
    constexpr bool simulate = true;

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate vector memory
    const uint32_t seedPtr = AppUtils::allocateVectorAndZero(32 * 2, vectorInitData);
    
    // Allocate scalar memory
    const uint32_t scalarSeedPtr = AppUtils::allocateScalarSeedAndInit(scalarInitData);
    const uint32_t scalarOperandPtr = AppUtils::loadScalars("stoch_rand_operands.bin", scalarInitData);
    const uint32_t readyFlagPtr = AppUtils::allocateScalarAndZero(4, scalarInitData);
    
    // Create RISC-V core with instruction and scalar data
    assert(scalarInitData.size() <= (128 * 1024));
    const auto code = generateCode(simulate, numVectorMultiply, seedPtr, scalarSeedPtr, scalarOperandPtr, readyFlagPtr);

    if(simulate) {
        RISCV riscV(code, scalarInitData);
    
        // Add vector co-processor
        riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
        // Run!
        riscV.run();
    
        const auto *scalarData = riscV.getScalarDataMemory().getData().data();
    
        // Write results to binary file
        std::ofstream out("out_stoch_round.bin", std::ios::binary);
        out.write(reinterpret_cast<const char*>(scalarData + scalarOperandPtr),
                  64 * 3 * numVectorMultiply);
    }
    else {
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

        // Read operands
        std::vector<uint8_t> operands(64 * 3 * numVectorMultiply);
        device.memcpyDataFromDevice(operands.data(), scalarOperandPtr, 64 * 3 * numVectorMultiply);
        
        // Write results to binary file
        std::ofstream out("out_stoch_round_device.bin", std::ios::binary);
        out.write(reinterpret_cast<const char*>(operands.data()), operands.size());
    }
    

}
