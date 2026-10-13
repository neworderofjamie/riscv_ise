// Standard C++ includes
#include <fstream>
#include <numeric>
#include <random>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>

// Third party includes
#include "third_party/CLI11.hpp"

// FeNN common includes
#include "fenn/common/app_utils.h"
#include "fenn/common/device.h"
#include "fenn/common/device_control.h"

// FeNN assembler includes
#include "fenn/assembler/assembler.h"
#include "fenn/assembler/assembler_utils.h"
#include "fenn/assembler/register_allocator.h"

// FeNN ISE includes
#include "fenn/ise/riscv.h"
#include "fenn/ise/vector_processor.h"

using namespace FeNN;

namespace
{
void checkOutput(volatile const uint32_t *wordData,
                 uint32_t timePtr)
{
    volatile const uint32_t *timeData = wordData + (timePtr / 4);
    
    for(int i = 0; i < 10; i++) {
        std::cout << *timeData++ << std::endl;
    }
}
}

int main(int argc, char** argv)
{
    
    bool device = false;
    int numCores = 1;
    int core = 0;
    plog::Severity logSeverity = plog::debug;

    CLI::App app{"Blank example"};
    app.add_option("-n,--num-cores", numCores, "Number of cores FeNN system has");
    app.add_option("-c,--core", core, "Which core to run on");
    app.add_flag("-d,--device", device, "Should be run on device rather than simulator");

    CLI11_PARSE(app, argc, argv);
    
    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    ::Common::Logging::init(logSeverity, logSeverity, 
                            &consoleAppender, &consoleAppender);
    FeNN::Common::Logging::init(logSeverity, logSeverity, logSeverity, logSeverity, logSeverity,
                                &consoleAppender, &consoleAppender, &consoleAppender, &consoleAppender, &consoleAppender);

    

    // Create memory contents
    std::vector<uint8_t> scalarInitData;
    std::vector<int16_t> vectorInitData;

    // Allocate scalar arrays
    const uint32_t readyFlagPtr = FeNN::Common::AppUtils::allocateScalarAndZero(4, scalarInitData);
    const uint32_t loopTimePtr = FeNN::Common::AppUtils::allocateScalarAndZero(4 * 10, scalarInitData);

    // Generate code
    const auto code = Assembler::Utils::generateStandardKernel(
        !device, readyFlagPtr,
        [=](Assembler::CodeGenerator &c, Assembler::VectorRegisterAllocator &vectorRegisterAllocator, 
            Assembler::ScalarRegisterAllocator &scalarRegisterAllocator)
        {
            ALLOCATE_SCALAR(SCount);
            ALLOCATE_SCALAR(SRemaining);
            ALLOCATE_SCALAR(SLoopTime);
            c.li(*SCount, 128);
            c.li(*SRemaining, 200);
            c.li(*SLoopTime, loopTimePtr);

            
            auto end = Assembler::createLabel();
            auto start = c.L();
            c.bltu(*SRemaining, *SCount, end);
            
            c.sw(*SRemaining, *SLoopTime);
            c.addi(*SLoopTime, *SLoopTime, 4);

            c.addi(*SRemaining, *SRemaining, -32);
            c.j_(start);
            c.L(end);


            /*4r : ADDI X16, X0, 256
            48 : BLTU X4, X16, 84
            52 : VLOAD V0, 0(X3)
            56 : VLOAD V2, 64(X15)
            60 : VADD_S V1, V1, V0
            64 : VSTORE V1, 0(X15)
            68 : VLOAD V0, 64(X3)
            72 : VLOAD V1, 128(X15)
            76 : VADD_S V2, V2, V0
            80 : VSTORE V2, 64(X15)
            84 : VLOAD V0, 128(X3)
            88 : VLOAD V2, 192(X15)
            92 : VADD_S V1, V1, V0
            96 : VSTORE V1, 128(X15)
            100 : VLOAD V0, 192(X3)
            104 : VLOAD V1, 256(X15)
            108 : VADD_S V2, V2, V0
            112 : VSTORE V2, 192(X15)
            116 : ADDI X15, X15, 256
            120 : ADDI X3, X3, 256
            124 : SUB X4, X4, X16
            128 : JAL X0, -80*/

            /* top-level loop structure
            ALLOCATE_SCALAR(STimeStart);
            ALLOCATE_SCALAR(STimeEnd);
            ALLOCATE_SCALAR(SLoopTime);
            auto endOfEvent = Assembler::createLabel();
            
            c.li(*STimeStart, 0);
            c.li(*STimeEnd, 10);
            c.li(*SLoopTime, loopTimePtr);

            auto timeLoop =c.L();
            {
                c.j_(endOfEvent);
                c.mv(*STimeStart, FeNN::Common::Reg::X0);
                c.mv(*STimeStart, FeNN::Common::Reg::X0);
                c.mv(*STimeStart, FeNN::Common::Reg::X0);
                c.mv(*STimeStart, FeNN::Common::Reg::X0);
                c.L(endOfEvent);

                c.sw(*STimeStart, *SLoopTime);
                c.addi(*SLoopTime, *SLoopTime, 4);

                c.addi(*STimeStart, *STimeStart, 1);
                c.bne(*STimeStart, *STimeEnd, timeLoop);
            }*/
        });

    // Dump to coe file
    //AppUtils::dumpCOE("blank_jump.coe", code);

    if(device) {
        LOGI << "Creating device";
        FeNN::Common::DeviceControl deviceControl(numCores);
        FeNN::Common::Device device(core, numCores);

        LOGI << "Resetting";
        // Put core into reset state
        deviceControl.setEnabled(false);
        
        LOGI << "Copying instructions (" << code.size() * sizeof(uint32_t) << " bytes)";
        device.uploadCode(code);
        
        LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
        device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());
        
        LOGI << "Enabling";
        // Put core into running state
        deviceControl.setEnabled(true);
        LOGI << "Running";
        
        // Wait until ready flag
        device.waitOnNonZero(readyFlagPtr);
        deviceControl.setEnabled(false);
        LOGI << "Done";

        volatile uint32_t *wordData = reinterpret_cast<volatile uint32_t*>(device.getDataMemory());
        checkOutput(wordData, loopTimePtr);

    }
    else {
        // Create RISC-V core with instruction and scalar data
        ISE::RISCV riscV;
        riscV.setInstructions(code);
        riscV.getScalarDataMemory().setData(scalarInitData);
        
        // Add vector co-processor
        riscV.addCoprocessor<ISE::VectorProcessor>(FeNN::Common::vectorQuadrant);
        
        // Run!
        if(!riscV.run()) {
            return 1;
        }

        auto *wordData = reinterpret_cast<uint32_t*>(riscV.getScalarDataMemory().getData());
        checkOutput(wordData, loopTimePtr);

    }
    return 0;

}
