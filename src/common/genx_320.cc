#include "common/genx_320.h"

// Standard C++ includes
#include <chrono>
#include <thread>

// PLOG includes
#include <plog/Log.h>

//----------------------------------------------------------------------------
// MIPI CSI register fields
//----------------------------------------------------------------------------
namespace 
{
namespace MIPICSI
{
struct Ctrl
{
    uint32_t enable :1;
    uint32_t empty :1;
    uint32_t busy :1;
    uint32_t frameSyncEn :1;
    uint32_t lineSyncEn :1;
    uint32_t:3;
    uint32_t channel :2;
    uint32_t dataType :6;
    uint32_t pktSize :14;
};

struct FrameCtrl
{
    uint32_t pktTimeoutEn :1;
    uint32_t pktFixRateEn :1;
    uint32_t pktFixSizeEn :1;
    uint32_t frameFixRateEn :1;
    uint32_t frameFixSizeEn :1;
    uint32_t fixRateEmptyPkt :1;
    uint32_t:26;
};

struct BLFrame 
{
    uint32_t val :24;
    uint32_t:6;
    uint32_t ckLaneHs :1;
    uint32_t enable :1;
};

struct StatCtrl
{
    uint32_t enable :1;
    uint32_t trigger :1;
    uint32_t clear :1;
};
}   // namespace MIPICSI

//----------------------------------------------------------------------------
// Bias generator register fields
//----------------------------------------------------------------------------
namespace BGen
{
struct Ctrl
{
    uint32_t burstTransferHvBank_0 :1;
    uint32_t burstTransferHvBank_1 :1;
    uint32_t burstTransferLvBank_0 :1;
    uint32_t burstTransferLvBank_1 :1;
    uint32_t biasRstnHv :1;
    uint32_t biasRstnLv :1;
};
}

//----------------------------------------------------------------------------
// EDF register fields
//----------------------------------------------------------------------------
namespace EDF
{
struct PipelineControl
{
	uint32_t enable :1;
	uint32_t dropNBackPressure :1;
	uint32_t bypass :1;
};

struct Control
{
	uint32_t format :2;
	uint32_t:2;
	uint32_t endianness :1;
};

struct OutputInterfaceControl
{
	uint32_t:4;
	uint32_t startOfFrameTimeout :12;
	uint32_t:16;
};

struct ExternalOutputAdapter
{
	uint32_t qosTimeout :16;
	uint32_t atomicQosMode :1;
};
}

//----------------------------------------------------------------------------
// SRAM register fields
//----------------------------------------------------------------------------
namespace SRAM
{
struct InitN
{
    uint32_t afkInitN :1;
    uint32_t ehcSTCInitN :1;
    uint32_t ercDLInitN :1;
    uint32_t ercIlgInitn :1;
    uint32_t ercTdropInitn :1;
    uint32_t mipiInitN :1;
    uint32_t cpiInitN :1;
    uint32_t imemInitN :1;
    uint32_t dmemInitN :1;
    uint32_t romInitN :1;
    uint32_t:22;
};

struct PD1
{
    uint32_t dmemPD :1;
    uint32_t imemPD :1;
    uint32_t romPD :1;
    uint32_t ercDlPd :1;
    uint32_t ercIlgPd :1;
    uint32_t ercTdropPd :1;
    uint32_t mipiPd :1;
    uint32_t cpPd :1;
};
}
}

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

    //------------------------------------------------------------------
    // Configure MIPI CSI-2 for 1-lane, 800 Mbps, variable-size packets
    //------------------------------------------------------------------
    // Frame ctrl: all zero (variable size)
    writeCamRegisterFields<MIPICSI::FrameCtrl>(RegisterAddress::MIPI_CSI_FRAME_CTRL,
                                               [](auto &s)
                                               {
                                                   s.pktTimeoutEn = 0;
                                                   s.pktFixRateEn = 0;
                                                   s.pktFixSizeEn = 0;
                                                   s.frameFixRateEn = 0;
                                                   s.frameFixSizeEn = 0;
                                                   s.fixRateEmptyPkt = 0;
                                               });
    
    // Packet size
    {
        uint32_t raw = readCamRegister(RegisterAddress::MIPI_CSI_CTRL);
        raw = (raw & ~(0x3FFF << 16)) | (0x1000 << 16);
        writeCamRegister(RegisterAddress::MIPI_CSI_CTRL, raw);
    }

    // EDF output interface control – start_of_frame_timeout
    writeCamRegisterFields<EDF::OutputInterfaceControl>(RegisterAddress::EDF_OUTPUT_INTERFACE_CONTROL,
                                                        [](auto &s){ s.startOfFrameTimeout = 0x271; });
    
    // EDF external output adapter
    writeCamRegisterFields<EDF::ExternalOutputAdapter>(RegisterAddress::EDF_EXTERNAL_OUTPUT_ADAPTER,
                                                       [](auto &s)
                                                       { 
                                                           s.qosTimeout = 0xFFFF;
                                                           s.atomicQosMode = 0;
                                                       });

    
    // Power up MIPI SRAM
    writeCamRegisterFields<SRAM::InitN>(RegisterAddress::SRAM_INITN,
                                        [](auto &s)
                                        { 
                                            s.mipiInitN = 1;
                                        });

    writeCamRegisterFields<SRAM::PD1>(RegisterAddress::SRAM_PD1,
                                      [](auto &s)
                                      { 
                                          s.mipiPd = 0;
                                      });

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
uint32_t GenX320::readCamRegister(RegisterAddress address)
{
    // Make combined transaction, writing 2-byte (big-endian) address and reading 4-byte payload
    const uint8_t addressBuffer[2] = {(static_cast<uint16_t>(address) >> 8) & 0xFF,
                                      static_cast<uint16_t>(address) & 0xFF};
    uint8_t dataBuffer[4];
    m_CamI2C.writeRead(addressBuffer, dataBuffer);

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
void GenX320::writeCamRegister(RegisterAddress address, uint32_t value, int numRetries)
{
    // Send 2-byte register address followed by 4-byte data (big-endian) 
    const uint8_t buffer[6] = {(static_cast<uint16_t>(address) >> 8) & 0xFF,
                               static_cast<uint16_t>(address) & 0xFF,
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
            LOGW << std::hex << "I2C write 0x" << static_cast<uint16_t>(address) << "=0x" << value << " failed (attempt " << std::dec << i + 1 << " / " << numRetries << ")";
        }
    }

    throw std::runtime_error("I2C write at 0x" + std::to_string(static_cast<uint16_t>(address)) + "failed after " + std::to_string(numRetries) + " retries");
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
