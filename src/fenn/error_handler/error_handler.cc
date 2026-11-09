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

int main(int argc, char** argv)
{
    
    int numCores = 1;
    int core = 0;
    plog::Severity logSeverity = plog::debug;

    CLI::App app{"Blank example"};
    app.add_option("-n,--num-cores", numCores, "Number of cores FeNN system has");
    app.add_option("-c,--core", core, "Which core to run on");

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

    // Generate code
    const auto code = Assembler::Utils::generateStandardKernel(
        false, readyFlagPtr,
        [=](Assembler::CodeGenerator &c, Assembler::VectorRegisterAllocator&, 
            Assembler::ScalarRegisterAllocator&)
        {
            c.div(FeNN::Common::Reg::X0, FeNN::Common::Reg::X0, FeNN::Common::Reg::X0);
        });

    // Dump to coe file
    FeNN::Common::AppUtils::dumpCOE("error_handler.coe", code);

    LOGI << "Creating device";
    FeNN::Common::DeviceControl deviceControl(numCores);
    FeNN::Common::Device device(core, numCores);

    LOGI << "Resetting";
    // Put core into reset state
    deviceControl.setEnabled(false);
    deviceControl.setBootAddress(FeNN::Common::bootAddress);
    
    LOGI << "Copying instructions (" << code.size() * sizeof(uint32_t) << " bytes)";
    device.uploadCode(code);
    
    LOGI << "Copying data (" << scalarInitData.size() << " bytes);";
    device.memcpyDataToDevice(0, scalarInitData.data(), scalarInitData.size());
    
    LOGI << "Enabling";
    // Put core into running state
    deviceControl.setEnabled(true);
    LOGI << "Running";
    
    // Wait until ready flag
    if(!device.waitOnNonZero(readyFlagPtr)) {
        const auto exceptionData = device.getExceptionData(4);
        LOGE << "Error running kernel cause:" << exceptionData.cause << ", PC:" << exceptionData.pc;        
    
    }
    deviceControl.setEnabled(false);
    LOGI << "Done";

    return 0;

}
