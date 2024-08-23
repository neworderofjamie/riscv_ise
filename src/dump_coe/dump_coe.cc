// Standard C++ includes
#include <fstream>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// RISC-V utils include
#include "common/app_utils.h"

// RISC-V assembler includes
#include "assembler/register_allocator.h"
#include "assembler/xbyak_riscv.hpp"


Xbyak_riscv::CodeGenerator generateCode()
{
    using namespace Xbyak_riscv;
    
    CodeGenerator c;
    VectorRegisterAllocator vectorRegisterAllocator;
    ScalarRegisterAllocator scalarRegisterAllocator;

    // Register allocation
    ALLOCATE_SCALAR(SX);
    ALLOCATE_SCALAR(SY);
    ALLOCATE_SCALAR(SZ);
    
    // Labels
    Label loop;

    c.li(*SX, 10);
    c.li(*SY, 0);
    c.li(*SZ, 0);

    // Outer word loop
    c.L(loop);
    {
        c.addi(*SX, *SX, -1);
        c.addi(*SY, *SY, 1);
        c.addi(*SZ, *SZ, 1);

        c.bne(*SX, Reg::X0, loop);
    }
    c.ecall();
    return c;
}


int main()
{
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    
    const auto code = generateCode().getCode();
    
    AppUtils::dumpCOE("output.coe", code);
}
