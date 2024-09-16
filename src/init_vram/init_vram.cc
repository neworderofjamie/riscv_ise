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
#include "assembler/register_allocator.h"
#include "assembler/xbyak_riscv.hpp"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"


Xbyak_riscv::CodeGenerator generateCode(uint32_t scalarPtr, uint32_t vectorPtr,
                                        uint32_t numVectors)
{
    using namespace Xbyak_riscv;
    
    CodeGenerator c;
    VectorRegisterAllocator vectorRegisterAllocator;
    ScalarRegisterAllocator scalarRegisterAllocator;

    AppUtils::generateScalarVectorMemCpy(c, vectorRegisterAllocator,
                                         scalarRegisterAllocator,
                                         scalarPtr, vectorPtr, numVectors);
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
    scalarInitData.resize(32 * 10 * 2);
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
    
    const auto &vectorData = riscV.getCoprocessor<VectorProcessor>(vectorQuadrant)->getVectorDataMemory().getData();
    
    std::ofstream out("out.txt");
    for(int16_t v : vectorData) {
        out << v << std::endl;
    }
    

}
