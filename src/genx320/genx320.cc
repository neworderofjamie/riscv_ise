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
     // Configure logging
    plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::debug, &consoleAppender);

    GenX320 genX320(GenX320::EventFormat::EVT2, "axi_gpio_0");
    // Create devices
    //MIPICSI2Receiver mipiReceiver("mipi_csi2_rx_subsyst_0");
    //AxisTkeepHandler axisTkeep("axis_tkeep_handler_0");
    //EventStreamSmartTracker eventStreamSmart("event_stream_smart_t_0");
    //UIO lowMemoryUIO("axi_bram_ctrl_0");
    //UIO highMemoryUIO("axi_bram_ctrl_1");

    genX320.powerOn();
}