#include "common/genx_320.h"

// Standard C++ includes
#include <chrono>
#include <thread>

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/genx320_reg.h"

//----------------------------------------------------------------------------
// GenX320
//----------------------------------------------------------------------------
GenX320::GenX320(EventFormat eventFormat, const std::string &gpioUIOName, 
                 const std::string &i2cPath, int muxSlaveAddress, int camSlaveAddress)
:   m_EventFormat(eventFormat), m_MuxI2C(i2cPath, muxSlaveAddress), 
    m_CamI2C(i2cPath, camSlaveAddress), m_GPIUIO(gpioUIOName)
{
}
//----------------------------------------------------------------------------
void GenX320::powerOn()
{
    using namespace std::chrono_literals;
    LOGI << "Power-on sequence startin";

    // Assert enable low
    m_GPIUIO.getData<uint32_t>()[0] = 0b00;
    std::this_thread::sleep_for(20ms);

    // Assert enable high
    m_GPIUIO.getData<uint32_t>()[0] = 0b11;
    std::this_thread::sleep_for(200ms);

    // Setup the I2C multiplexer to allow all channels to be reached
    m_MuxI2C.write(uint8_t{0xFF});

    LOGD << "Pulsing Digital and CPU resets...";

    // Assert
    writeCamRegister<DigSoftReset>(0x05);
    writeCamRegister<Mbx::CpuSoftReset>(0x01);
    std::this_thread::sleep_for(20ms);

    // Release
    writeCamRegister<DigSoftReset>(0x04);
    writeCamRegister<Mbx::CpuSoftReset>(0x00);
    std::this_thread::sleep_for(10ms);

    // Wait for camera to boot
    waitBoot();

    // Read and verify the chip ID register.
    const uint32_t cid = readCamRegister<ChipId>();
    if(cid != chipIDExpected) {
        throw std::runtime_error("GenX320 chip ID mismatch: got 0x" + std::to_string(cid) +
                                 ", expected 0x" + std::to_string(chipIDExpected));
    }
    LOGI << std::hex << "Chip ID OK: 0x" << cid;

    //------------------------------------------------------------------
    // Configure MIPI CSI-2 for 1-lane, 800 Mbps, variable-size packets
    //------------------------------------------------------------------
    // Frame ctrl: all zero (variable size)
    writeCamRegisterFields<MipiCsi::FrameCtrl>([](auto &s)
                                               {
                                                   s.pkt_timeout_en = 0;
                                                   s.pkt_fix_rate_en = 0;
                                                   s.pkt_fix_size_en = 0;
                                                   s.frame_fix_rate_en = 0;
                                                   s.frame_fix_size_en = 0;
                                                   s.fix_rate_empty_pkt = 0;
                                               });
    
    // Packet size
    {
        uint32_t raw = readCamRegister<MipiCsi::Ctrl>();
        raw = (raw & ~(0x3FFF << 16)) | (0x1000 << 16);
        writeCamRegister<MipiCsi::Ctrl>(raw);
    }

    // EDF output interface control – start_of_frame_timeout
    writeCamRegisterFields<EDF::OutputInterfaceControl>([](auto &s){ s.start_of_frame_timeout = 0x271; });
    
    // EDF external output adapter
    writeCamRegisterFields<EDF::ExternalOutputAdapter>([](auto &s)
                                                       { 
                                                           s.qos_timeout = 0xFFFF;
                                                           s.atomic_qos_mode = 0;
                                                       });

    
    // Power up MIPI SRAM
    writeCamRegisterFields<SRAM::InitN>([](auto &s){ s.mipi_initn = 1; });

    writeCamRegisterFields<SRAM::Pd1>([](auto &s){ s.mipi_pd = 0; });

    // Enable MIPI CSI
    //self.write_field("mipi_csi_ctrl", "enable", 1)
    
    // Blanking frame register
    //self.write_reg(REGISTERS["mipi_csi_bl_frame"]["addr"], 0x80003E80)
    //self.write_field("mipi_csi_stat_ctrl", "enable", 1)
    
    // Configure FPGA-side MIPI RX if provided
    //self._configure_fpga_mipi()
    
    LOGD << "MIPI CSI-2 configured (1 lane, 800 Mbps, variable-size)";
    
    setEventFormat(m_EventFormat);
    
    //self._tune_analog()
    //self._roi_window_init()
    //self._erc_init()
    //self._bias_init()
}
//----------------------------------------------------------------------------
void GenX320::setEventFormat(EventFormat eventFormat)
{
    m_EventFormat = eventFormat;


    /*self.write_field("edf_control", "format", EVT_FMT[fmt])
    self.write_field("edf_control", "endianness", 0)  # little-endian
    self.write_field("edf_pipeline_control", "bypass", 0)
    self.write_field("edf_pipeline_control", "enable", 1)
    
    log.info("Event format set to %s", fmt)*/
}
//----------------------------------------------------------------------------
void GenX320::waitBoot(int numRetries)
{
    using namespace std::chrono_literals;
    LOGI << "Waiting for sensor boot magic...";
    for(int i = 0; i < numRetries; i++) {
        const uint32_t magic = readCamRegister<Mbx::Misc>();
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


    const uint32_t magic = readCamRegister<Mbx::Misc>();
    if(magic != bootMagic) {
        // Diagnostic: read some other registers to see if digital core is alive
        const uint32_t cid = readCamRegister<ChipId>();
        const uint32_t dsr = readCamRegister<DigSoftReset>();
        const uint32_t scc = readCamRegister<SysClkCtrl>();
        
        LOGE << std::hex << "GenX320 boot magic mismatch: got 0x" << magic << ", expected 0x" << bootMagic << ".";
        LOGE << std::hex << "Diagnostics: ChipID=" << cid << ", DigReset=0x" << dsr << ", SysClk=0x" << scc;
        throw std::runtime_error("Unable to boot camera");
    }

    LOGI << std::hex << "Boot magic OK (0x" << magic << ")";
}
//----------------------------------------------------------------------------
uint32_t GenX320::readCamRegister(uint16_t address)
{
    // Make combined transaction, writing 2-byte (big-endian) address and reading 4-byte payload
    const uint8_t addressBuffer[2] = {(address >> 8) & 0xFF,
                                      address & 0xFF};
    uint8_t dataBuffer[4];
    m_CamI2C.writeRead(addressBuffer, dataBuffer);

    // Re-assemble little-endian data
    const uint32_t val = ((dataBuffer[0] << 24) |
                          (dataBuffer[1] << 16) |
                          (dataBuffer[2] << 8) |
                          dataBuffer[3]);
    if(val == 0xBAADF00D) {
        LOGE << std::hex << "Bus Error: I2C read at 0x" << address << " returned 0xBAADF00D. This usually means the sensor is not responding or the IIC bridge failed.";
    }

    return val;
}
//----------------------------------------------------------------------------
void GenX320::writeCamRegister(uint16_t address, uint32_t value, int numRetries)
{
    // Send 2-byte register address followed by 4-byte data (big-endian) 
    const uint8_t buffer[6] = {(address >> 8) & 0xFF,
                               address & 0xFF,
                               (value >> 24) & 0xFF,
                               (value >> 16) & 0xFF,
                               (value >> 8) & 0xFF,
                               value & 0xFF};
    for(int i = 0; i < numRetries; i++) {
        try {
            m_CamI2C.write(buffer);
            return;
        }
        catch(std::runtime_error &e) {
            LOGW << std::hex << "I2C write 0x" << address << "=0x" << value << " failed (attempt " << std::dec << i + 1 << " / " << numRetries << ")";
        }
    }

    throw std::runtime_error("I2C write at 0x" + std::to_string(address) + "failed after " + std::to_string(numRetries) + " retries");
}

