#include "common/axis_tkeep_handler.h"
#include "common/event_stream_smart_tracker.h"
#include "common/mipi_csi2_receiver.h"
#include "common/uio.h"

int main()
{
    // Create devices
    MIPICSI2Receiver mipiReceiver("mipi_csi2_rx_subsyst_0");
    AxisTkeepHandler axisTkeep("axis_tkeep_handler_0");
    EventStreamSmartTracker eventStreamSmart("event_stream_smart_t_0");
    UIO lowMemoryUIO("axi_bram_ctrl_0");
    UIO highMemoryUIO("axi_bram_ctrl_1");
}