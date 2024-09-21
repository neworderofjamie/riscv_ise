// Standard C++ includes
#include <fstream>
#include <numeric>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V utils include
#include "common/app_utils.h"

// RISC-V assembler includes
#include "assembler/assembler.h"
#include "assembler/register_allocator.h""

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"


CodeGenerator generateCode(uint32_t scalarPtr, uint32_t vectorPtr,
                           uint32_t numVectors)
{
    CodeGenerator c;
    VectorRegisterAllocator vectorRegisterAllocator;
    ScalarRegisterAllocator scalarRegisterAllocator;

    // Generate memcpy from scalarPtr to vectorPtr
    AppUtils::generateScalarVectorMemcpy(c, vectorRegisterAllocator,
                                         scalarRegisterAllocator,
                                         scalarPtr, vectorPtr, numVectors);

    // Generate memcpy back from vectorPtr to scalarPtr + (numVectors * 64)
    AppUtils::generateVectorScalarMemcpy(c, vectorRegisterAllocator,
                                         scalarRegisterAllocator,
                                         vectorPtr, scalarPtr + (numVectors * 64), 
                                         numVectors);
    c.ecall();
    return c;
}


int main()
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;
    scalarInitData.resize(32 * 10 * 4);
    vectorInitData.resize(32 * 10);
    
    // Generate 10 vectors of increasing numbers and copy into scalar memory
    std::vector<int16_t> test(10 * 32);
    std::iota(test.begin(), test.end(), 0);
    std::memcpy(scalarInitData.data(), test.data(), 10 * 32 * 2);

    // Create RISC-V core with instruction and scalar data
    const auto code = generateCode(0, 0, 10).getCode();

    AppUtils::dumpCOE("init_vram.coe", code);

    RISCV riscV(code, scalarInitData);
    
    // Add vector co-processor
    riscV.addCoprocessor<VectorProcessor>(vectorQuadrant, vectorInitData);
    
    // Run!
    riscV.run();
    
    // Verify copy was successful
    const auto *scalarData = reinterpret_cast<const int16_t*>(riscV.getScalarDataMemory().getData().data());
    for(size_t i = 0; i < (32 * 10); i++) {
        const int16_t read = scalarData[i];
        const int16_t write = scalarData[i + (32 * 10)];
        std::cout << read << "(" << write << "), ";
        assert(read == write);
    }
    std::cout << std::endl;
    
    

}
