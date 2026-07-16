#include "common/device_control.h"

// Standard C++ includes
#include <chrono>
#include <fstream>
#include <stdexcept>
#include <string>

// Standard C includes
#include <cassert>
#include <cstring>

// POSIX includes
#ifdef __linux__ 
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
    #include <sys/mman.h>
#endif

// PLOG includes
#include <plog/Log.h>

// RISC-V common includes
#include "common/utils.h"

//----------------------------------------------------------------------------
// DeviceControl
//----------------------------------------------------------------------------
DeviceControl::DeviceControl(int numCores)
{
#ifdef __linux__ 
    // **TODO** fix names
    const std::string targetNamePrefix = (numCores == 1) ? "" : "core_0_";
    LOGI << "Creating DeviceControl for " << numCores << " system";
    
    // Create UIO
    m_GPIOUIO = std::make_unique<UIO>(targetNamePrefix + "axi_gpio");

    // Create MIPI CSI2 receiver
    try {
        m_MIPICSI2Receiver = std::make_unique<MIPICSI2Receiver>("mipi_csi2_rx_subsyst_0");
    }
    catch (const std::runtime_error &) {
        LOGW << "MIPI CSI 2 Receiver not found - GenX320 event camera not available";
    }

    // IF MIPI CSI 2 receiver is successfully initialiser, create camera
    if (m_MIPICSI2Receiver) {
        m_GenX320 = std::make_unique<GenX320>(EventFormat::EVT2, "axi_gpio_cam", 
                                              m_MIPICSI2Receiver.get());
    }
#else
    throw std::runtime_error("Device control interface only supports Linux");
#endif  // __linux__
}
//----------------------------------------------------------------------------
void DeviceControl::setEnabled(bool enabled)
{
    // Channel 1 AXI GPIO Data Register
    volatile uint32_t *gpio = getGPIO();
    gpio[0] = enabled ? 0xFFFFFFFF : 0x0;
}
//----------------------------------------------------------------------------
void DeviceControl::setILATrigger(bool enabled)
{
    // Channel 2 AXI GPIO Data Register.
    volatile uint32_t *gpio = getGPIO();
    gpio[2] = enabled ? 0xFFFFFFFF : 0x0;
}

