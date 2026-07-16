// Standard C++ includes
#include <chrono>
#include <fstream>
#include <thread>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// Common includes
#include "common/device_control.h"

int main()
{
    using namespace std::chrono_literals;

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);
    
    DeviceControl deviceControl(1);

    LOGI << "Starting MIPI status: activate lanes = " << deviceControl.getMIPICSI2Receiver()->getActiveLanes() << " SLB full="<< deviceControl.getMIPICSI2Receiver()->isStreamLineBufferFull() << ", SPFNF=" << deviceControl.getMIPICSI2Receiver()->isShortPacketFIFONotempty() << ", SPFF=" << deviceControl.getMIPICSI2Receiver()->isShortPacketFIFOFull();
    LOGI << "Starting MIPI packet count: " << deviceControl.getMIPICSI2Receiver()->getPacketCount();
    deviceControl.setEnabled(true);
    deviceControl.getGenX320()->startStreaming();
    
    std::this_thread::sleep_for(2s);
    
    deviceControl.getGenX320()->stopStreaming();
    LOGI << "Ending MIPI status: activate lanes = " << deviceControl.getMIPICSI2Receiver()->getActiveLanes() << " SLB full="<< deviceControl.getMIPICSI2Receiver()->isStreamLineBufferFull() << ", SPFNF=" << deviceControl.getMIPICSI2Receiver()->isShortPacketFIFONotempty() << ", SPFF=" << deviceControl.getMIPICSI2Receiver()->isShortPacketFIFOFull();
    LOGI << "Ending MIPI packet count: " << deviceControl.getMIPICSI2Receiver()->getPacketCount();
}
