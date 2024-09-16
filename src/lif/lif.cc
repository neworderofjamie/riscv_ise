// Standard C++ includes
#include <fstream>
#include <numeric>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V common includes
#include "common/app_utils.h"

// RISC-V assembler includes
#include "assembler/register_allocator.h"
#include "assembler/xbyak_riscv.hpp"

// RISC-V ISE includes
#include "ise/riscv.h"
#include "ise/vector_processor.h"


Xbyak_riscv::CodeGenerator generateCode()
{
    using namespace Xbyak_riscv;
    
    CodeGenerator c;
    VectorRegisterAllocator vectorRegisterAllocator;
    ScalarRegisterAllocator scalarRegisterAllocator;

    // Generate code to copy vector of currents from scalar memory to vector memory
    AppUtils::generateScalarVectorMemCpy(c, vectorRegisterAllocator, scalarRegisterAllocator,
                                         0, 0, 1);

    // Register allocation
    ALLOCATE_SCALAR(SVBuffer);
    ALLOCATE_SCALAR(SVBufferEnd);
    ALLOCATE_VECTOR(VAlpha);
    ALLOCATE_VECTOR(VV);
    ALLOCATE_VECTOR(VVReset);
    ALLOCATE_VECTOR(VVThresh);
    ALLOCATE_VECTOR(VI);

    // alpha = e^(-1/20)
    c.vlui(*VAlpha, 7792);
    
    // v = 0
    c.vlui(*VV, 0);
    
    // v_thresh = 1
    c.vlui(*VVThresh, 8192);

    // v_reset = 0
    c.vlui(*VVReset, 0);
    
    // i = vmem[0..32]
    c.vloadv(*VI, Reg::X0);
   
    // Start writing 64 bytes in (after I values)
    c.li(*SVBuffer, 64);

    // End writing at 100 timesteps * 64 bytes
    c.li(*SVBufferEnd, 6400);
    
    // Loop over time
    Label loop;
    c.L(loop);
    {
        // Register allocation
        ALLOCATE_SCALAR(SSpike);

        // v *= alpha
        c.vmul(13, *VV, *VV, *VAlpha);
    
        // v += i
        c.vadd(*VV, *VV, *VI);
    
        // spike = VV >= VThres
        c.vtge(*SSpike, *VV, *VVThresh);
    
        // v = spk ? v_reset : v
        c.vsel(*VV, *SSpike, *VVReset);
    
        //vmem[a...a+32] = v
        c.vstore(*VV, *SVBuffer);
        c.addi(*SVBuffer, *SVBuffer, 64);
    
        // While x2 (address) < x1 (count), goto loop
        c.bne(*SVBuffer, *SVBufferEnd, loop);
    }
    
    c.ecall();
    return c;
}


int main()
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    // Create memory contents
    std::vector<uint8_t> scalarInitData(4096);
    std::vector<int16_t> vectorInitData(4096);
    
    // Copy increasing input currents into scalar memory
    std::vector<int16_t> test{0, 26, 53, 79, 106, 132, 159, 185, 211, 238, 264, 291, 317, 344, 370, 396, 423, 449,
                              476, 502, 529, 555, 581, 608, 634, 661, 687, 713, 740, 766, 793, 819};
    std::memcpy(scalarInitData.data(), test.data(), test.size() * 2);

    // Generate code
    const auto code = generateCode().getCode();

    // Dump to coe file
    AppUtils::dumpCOE("lif.coe", code);

    // Create RISC-V core with instruction and scalar data
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
