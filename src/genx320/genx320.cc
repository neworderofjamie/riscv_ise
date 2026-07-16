// Standard C++ includes
#include <chrono>
#include <fstream>
#include <thread>

// PLOG includes
#include <plog/Log.h>
#include <plog/Severity.h>
#include <plog/Appenders/ConsoleAppender.h>

// Common includes
#include "common/axis_tkeep_handler.h"
#include "common/event_stream_smart_tracker.h"
#include "common/genx_320.h"
#include "common/mipi_csi2_receiver.h"
#include "common/uio.h"

int main()
{
    using namespace std::chrono_literals;

    // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    UIO memoryUIO("axi_bram_ctrl_0");

    // Initially zero memory
    {
        volatile uint32_t *data = memoryUIO.getData<uint32_t>();
        for(int i = 0; i < 2048; i++) {
            *data++ = 0;
        }
    }
    
    MIPICSI2Receiver mipiReceiver("mipi_csi2_rx_subsyst_0");

    GenX320 genX320(EventFormat::EVT2, "axi_gpio_0", &mipiReceiver);
    
    LOGI << "Starting MIPI packet count: " << mipiReceiver.getPacketCount();
    LOGI << "Starting core status:" << std::hex << mipiReceiver.readReg(MIPICSI2Receiver::Register::CORE_STATUS);
    LOGI << "Starting int status:" << std::hex << mipiReceiver.readReg(MIPICSI2Receiver::Register::INT_STATUS);
    genX320.startStreaming();
    
    std::this_thread::sleep_for(2s);
    
    genX320.stopStreaming();
    LOGI << "Ending MIPI packet count: " << mipiReceiver.getPacketCount();
    LOGI << "Ending core status:" << std::hex << mipiReceiver.readReg(MIPICSI2Receiver::Register::CORE_STATUS);  
    LOGI << "Ending int status:" << std::hex << mipiReceiver.readReg(MIPICSI2Receiver::Register::INT_STATUS);
    
    std::vector<uint32_t> dataVector;
    dataVector.reserve(2048);
    
    {
        volatile const uint32_t *data = memoryUIO.getData<uint32_t>();
        for(int i = 0; i < 2048; i++) {
            dataVector.push_back((uint32_t)*data++);
        }
    }
    
    std::ofstream outputFile("genx320.raw", std::ios::binary);
    outputFile << "% evt 2.0" << std::endl;
    outputFile << "% format EVT2;height=320;width=320" << std::endl;
    outputFile << "% end" << std::endl;
    outputFile.write(reinterpret_cast<char*>(dataVector.data()), 4 * dataVector.size());
}
