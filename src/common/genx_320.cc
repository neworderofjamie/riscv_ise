#include "common/genx_320.h"

// Standard C++ includes
#include <chrono>
#include <thread>

// PLOG includes
#include <plog/Log.h>

//----------------------------------------------------------------------------
// GenX320
//----------------------------------------------------------------------------
GenX320::GenX320(const std::string &gpioUIOName, const std::string &i2cPath, 
                 int muxSlaveAddress, int camSlaveAddress)
:   m_MuxI2C(i2cPath, muxSlaveAddress), m_CamI2C(i2cPath, camSlaveAddress), m_GPIUIO(gpioUIOName)
{
}
//----------------------------------------------------------------------------
void GenX320::powerOn()
{
    using namespace std::chrono_literals;
    LOGI << "Power-on sequence startin";

    // Assert enable low
    m_GPIUIO.getData<uint32_t>()[0] = 0;
    std::this_thread::sleep_for(20ms);

    // Assert enable high
    m_GPIUIO.getData<uint32_t>()[0] = 1;
    std::this_thread::sleep_for(200ms);

    // Setup the I2C multiplexer to allow all channels to be reached
    m_MuxI2C.write(uint8_t{0xFF});

    LOGD << "Pulsing Digital and CPU resets...";

    // Assert
    writeCamRegister(RegisterAddress::DIG_SOFT_RESET, 0x05);
    writeCamRegister(RegisterAddress::MBX_CPU_SOFT_RESET, 0x01);
    std::this_thread::sleep_for(20ms);

    // Release
    writeCamRegister(RegisterAddress::DIG_SOFT_RESET, 0x04);
    writeCamRegister(RegisterAddress::MBX_CPU_SOFT_RESET, 0x00);
    std::this_thread::sleep_for(10ms);

    // Wait for camera to boot
    waitBoot();

    // Read and verify the chip ID register.
    const uint32_t cid = readCamRegister(RegisterAddress::CHIP_ID);
    if(cid != chipIDExpected) {
        throw std::runtime_error("GenX320 chip ID mismatch: got 0x" + std::to_string(cid) +
                                 ", expected 0x" + std::to_string(chipIDExpected));
    }
    LOGI << std::hex << "Chip ID OK: 0x" << cid;
}
//----------------------------------------------------------------------------
uint32_t GenX320::readCamRegister(RegisterAddress address)
{
    // Send 2-byte register address (big-endian) 
    const uint8_t addressBuffer[2] = {(static_cast<uint16_t>(address) >> 8) & 0xFF,
                                      static_cast<uint16_t>(address) & 0xFF};
    m_CamI2C.write(addressBuffer);

    // Read 4 byte response
    uint8_t dataBuffer[4];
    m_CamI2C.read(dataBuffer);

    // Re-assemble little-endian data
    const uint32_t val = ((dataBuffer[0] << 24) |
                          (dataBuffer[1] << 16) |
                          (dataBuffer[2] << 8) |
                          dataBuffer[3]);
    if(val == 0xBAADF00D) {
        LOGE << std::hex << "Bus Error: I2C read at 0x" << static_cast<uint16_t>(address) << " returned 0xBAADF00D. This usually means the sensor is not responding or the IIC bridge failed.";
    }

    return val;
}
//----------------------------------------------------------------------------
void GenX320::writeCamRegister(RegisterAddress address, uint32_t value)
{
    // Send 2-byte register address followed by 4-byte data (big-endian) 
    const uint8_t buffer[6] = {(static_cast<uint16_t>(address) >> 8) & 0xFF,
                               static_cast<uint16_t>(address) & 0xFF,
                               (value >> 24) & 0xFF,
                               (value >> 16) & 0xFF,
                               (value >> 8) & 0xFF,
                               value & 0xFF};
    m_CamI2C.write(buffer);
}
//----------------------------------------------------------------------------
void GenX320::waitBoot(int numRetries)
{
    using namespace std::chrono_literals;
    LOGI << "Waiting for sensor boot magic...";
    for(int i = 0; i < numRetries; i++) {
        const uint32_t magic = readCamRegister(RegisterAddress::MBX_MISC);
        if(magic == bootMagic) {
            LOGI << std::hex << "Boot magic OK (0x" << magic << ") after " << std::dec << i << " retries";
            return;
        }
        else if(magic == 0xBAADF00D) {
            // Bus error, maybe I2C controller is still settling?
            continue;
        }
        std::this_thread::sleep_for(20ms);
    }


    const uint32_t magic = readCamRegister(RegisterAddress::MBX_MISC);
    if(magic != bootMagic) {
        // Diagnostic: read some other registers to see if digital core is alive
        const uint32_t cid = readCamRegister(RegisterAddress::CHIP_ID);
        const uint32_t dsr = readCamRegister(RegisterAddress::DIG_SOFT_RESET);
        const uint32_t scc = readCamRegister(RegisterAddress::SYS_CLK_CTRL);
        
        LOGE << std::hex << "GenX320 boot magic mismatch: got 0x" << magic << ", expected 0x" << bootMagic << ".";
        LOGE << std::hex << "Diagnostics: ChipID=" << cid << ", DigReset=0x" << dsr << ", SysClk=0x" << scc;
        throw std::runtime_error("Unable to boot camera");
    }

    LOGI << std::hex << "Boot magic OK (0x" << magic << ")";
}