#include "common/genx_320.h"

// Standard C++ includes
#include <chrono>
#include <thread>

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/genx320_reg.h"
#include "common/mipi_csi2_receiver.h"

namespace
{
struct BiasDefault
{
    uint16_t address;
    uint32_t value;
};

BiasDefault factoryBiasDefaults[] = {
    {0x1000, 0x0301003D},  // BIAS_PR_HV0
    {0x1004, 0x03010022},  // BIAS_FO_HV0
    {0x1008, 0x0101003F},  // BIAS_FES_HV0
    {0x1100, 0x03010028},  // BIAS_HPF_LV0
    {0x1104, 0x01010019},  // BIAS_DIFF_ON_LV0
    {0x1108, 0x01010033},  // BIAS_DIFF_LV0
    {0x110C, 0x0101001C},  // BIAS_DIFF_OFF_LV0
    {0x1110, 0x01010039},  // BIAS_INV_LV0
    {0x1114, 0x0309000A},  // BIAS_REFR_LV0
    {0x1118, 0x03010038},  // BIAS_INVP_LV0
    {0x111C, 0x03000074},  // BIAS_REQ_PU_LV0
    {0x1120, 0x010000A4}}; // BIAS_SM_PDY_LV0
}
//----------------------------------------------------------------------------
// GenX320
//----------------------------------------------------------------------------
GenX320::GenX320(EventFormat eventFormat, const std::string &gpioUIOName, 
                 MIPICSI2Receiver *mipiCSI2Receiver, const std::string &i2cPath, int muxSlaveAddress, int camSlaveAddress)
:   m_EventFormat(eventFormat), m_MuxI2C(i2cPath, muxSlaveAddress), 
    m_CamI2C(i2cPath, camSlaveAddress), m_GPIUIO(gpioUIOName), 
    m_MIPICSI2Receiver(mipiCSI2Receiver), m_Streaming(false)
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
    writeReg<DigSoftReset>(0x05);
    writeReg<Mbx::CpuSoftReset>(0x01);
    std::this_thread::sleep_for(20ms);

    // Release
    writeReg<DigSoftReset>(0x04);
    writeReg<Mbx::CpuSoftReset>(0x00);
    std::this_thread::sleep_for(10ms);

    // Wait for camera to boot
    waitBoot();

    // Read and verify the chip ID register.
    const uint32_t cid = readReg<ChipId>();
    if(cid != chipIDExpected) {
        throw std::runtime_error("GenX320 chip ID mismatch: got 0x" + std::to_string(cid) +
                                 ", expected 0x" + std::to_string(chipIDExpected));
    }
    LOGI << std::hex << "Chip ID OK: 0x" << cid;

    //------------------------------------------------------------------
    // Configure MIPI CSI-2 for 1-lane, 800 Mbps, variable-size packets
    //------------------------------------------------------------------
    // Frame ctrl: all zero (variable size)
    writeRegFields<MipiCsi::FrameCtrl>([](auto &s)
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
        uint32_t raw = readReg<MipiCsi::Ctrl>();
        raw = (raw & ~(0x3FFF << 16)) | (0x1000 << 16);
        writeReg<MipiCsi::Ctrl>(raw);
    }

    // EDF output interface control – start_of_frame_timeout
    writeRegFields<EDF::OutputInterfaceControl>([](auto &s){ s.start_of_frame_timeout = 0x271; });
    
    // EDF external output adapter
    writeRegFields<EDF::ExternalOutputAdapter>([](auto &s)
                                               { 
                                                   s.qos_timeout = 0xFFFF;
                                                   s.atomic_qos_mode = 0;
                                               });

    
    // Power up MIPI SRAM
    writeRegFields<SRAM::InitN>([](auto &s){ s.mipi_initn = 1; });

    writeRegFields<SRAM::Pd1>([](auto &s){ s.mipi_pd = 0; });

    // Enable MIPI CSI
    writeRegFields<MipiCsi::Ctrl>([](auto &s){ s.enable = 1; });
    
    // Blanking frame register
    writeReg<MipiCsi::BlFrame>(0x80003E80);
    writeRegFields<MipiCsi::Stat::Ctrl>([](auto &s){ s.enable = 1; });
    
    // Configure FPGA-side MIPI RX if provided
    if(m_MIPICSI2Receiver) {
        LOGI << "Configuring FPGA-side MIPI CSI-2 RX Subsystem...";
        
        // 1. Enable Core
        m_MIPICSI2Receiver->setCoreEnabled(true);
        
        // 2. Set active lanes to 1 (00 = 1 lane)
        // Note: Even if hardware is 1-lane, setting this ensures consistency.
        m_MIPICSI2Receiver->setLanes(1, 1);
        
        // 3. Clear interrupts
        m_MIPICSI2Receiver->setInterruptStatus(0xFFFFFFFF);
        
        LOGI << "FPGA-side MIPI RX configured and enabled";
    }
    else {
        LOGD << "No FPGA MIPI RX instance provided; skipping FGPA-side config.";
    }

    LOGD << "MIPI CSI-2 configured (1 lane, 800 Mbps, variable-size)";
    
    setEventFormat(m_EventFormat);
    
    //------------------------------------------------------------------------
    // Write factory-default bias values and burst-transfer them.
    //------------------------------------------------------------------------
    // Enable bias reset lines
    writeRegFields<BgenCtrl>([](auto &s)
                             {
                                 s.bias_rstn_hv = 1;
                                 s.bias_rstn_lv = 1;
                             });
    
    std::this_thread::sleep_for(200us);

    // Clear ibtype_sel on DIFF_ON and DIFF_OFF (vdac type)
    {
        // ibtype_sel bit in bgen register layout
        const uint32_t ibtypeMask = (1 << 19)  ;

        clearRegBits(0x1104, ibtypeMask);   // BIAS_DIFF_ON_LV0
        clearRegBits(0x110C, ibtypeMask);   // BIAS_DIFF_OFF_LV0
    }
    
    // Write all factory defaults
    for(const auto &f : factoryBiasDefaults) {
        writeReg(f.address, f.value);
    }

    // Burst-transfer both HV and LV bank 0
    writeRegFields<BgenCtrl>([](auto &s)
                             {
                                 s.burst_transfer_hv_bank_0 = 1;
                                 s.burst_transfer_hv_bank_1 = 0;
                                 s.burst_transfer_lv_bank_0 = 1;
                                 s.burst_transfer_lv_bank_1 = 0;
                                 s.bias_rstn_hv = 1;
                                 s.bias_rstn_lv = 1;
                             });
    
    LOGD << "Analog bias tuning complete";
    
    //------------------------------------------------------------------------
    // Set default ROI
    //------------------------------------------------------------------------
    writeRegFields<ROI::Ctrl>([](auto &s)
                              {
                                  s.roi_td_en = 1;
                                  s.px_iphoto_en = 0;
                                  s.px_sw_rstn = 1;
                                  s.td_shadow_trigger = 0;
                              });
    writeRegFields<ROI::MasterChickenBit>([](auto &s){ s.driver_register_if_en = 0; });
    writeRegFields<ROI::Ctrl>([](auto &s){ s.px_roi_halt_programming = 0; });
    writeRegFields<ROI::MasterCtrl>([](auto &s)
                                    {
                                        s.master_en = 1;
                                        s.master_run = 0;
                                    });
    resetROI();


    //self._erc_init()
    //self._bias_init()
}
//----------------------------------------------------------------------------
GenX320::~GenX320()
{
    using namespace std::chrono_literals;

    if(m_Streaming) {
        stopStreaming();
        std::this_thread::sleep_for(15ms);
    }

    m_GPIUIO.getData<uint32_t>()[0] = 0b00;
    std::this_thread::sleep_for(1ms);

    LOGI << "GenX320 powered off";
}
//----------------------------------------------------------------------------
void GenX320::startStreaming(StreamingSource source)
{
    // Enable MIPI
    writeRegFields<MipiCsi::Ctrl>([](auto &s){ s.enable = 1; });
    
    // Enable LP output
    writeRegFields<Readout::LpCtrl>([](auto &s){ s.lp_output_disable = 0; });
    
    // Enable time base
    writeRegFields<Readout::TimeBaseCtrl>([](auto &s){ s.time_base_enable = 1; });

    if(source == +StreamingSource::PIXEL_ARRAY) {
        writeRegFields<Readout::ReadoutCtrl>([](auto &s)
                                             {
                                                 s.ro_self_test_en = 0;
                                                 s.ro_digital_pipe_en = 1;
                                             });

        writeRegFields<Readout::TdCtrl>([](auto &s)
                                        {
                                            s.ro_td_ack_y_rstn = 1;
                                            s.ro_td_arb_y_rstn = 1;
                                            s.ro_td_addr_y_rstn = 1;
                                            s.ro_td_sendreq_y_rstn = 1;
                                            s.ro_td_int_x_rstn = 1;
                                            s.ro_td_int_y_rstn = 1;
                                        });
        writeRegFields<ROI::Ctrl>([](auto &s)
                                  {
                                      s.px_sw_rstn = 1;
                                      s.roi_td_en = 1;
                                  });
    }
    else if(source == +StreamingSource::RO_PATTERN) {
        writeRegFields<Readout::ReadoutCtrl>([](auto &s)
                                             {
                                                 s.ro_self_test_en = 1;
                                                 s.ro_digital_pipe_en = 1;
                                             });
    }
    else if(source == +StreamingSource::TS_PATTERN) {
        writeReg<Readout::ReadoutCtrl>(0);
    }
    
    LOGI << "Streaming started (source="<< source._to_string() << ")";
    m_Streaming = true;
}
//----------------------------------------------------------------------------
void GenX320::stopStreaming()
{
    using namespace std::chrono_literals;

    assert(m_Streaming);

    // Disable pixel readout
    writeRegFields<ROI::Ctrl>([](auto &s){ s.px_sw_rstn = 0; });
    writeRegFields<Readout::TdCtrl>([](auto &s)
                                    {
                                        s.ro_td_ack_y_rstn = 0;
                                        s.ro_td_arb_y_rstn = 0;
                                        s.ro_td_addr_y_rstn = 0;
                                        s.ro_td_sendreq_y_rstn = 0;
                                    });
                        
    // Disable LP
    writeRegFields<Readout::LpCtrl>([](auto &s)
                                    {
                                        s.lp_output_disable = 1; 
                                        s.lp_keep_th = 0;
                                    });
    std::this_thread::sleep_for(1ms);

    // Disable time base
    writeRegFields<Readout::TimeBaseCtrl>([](auto &s){ s.time_base_enable = 0; });

    // Disable MIPI
    writeRegFields<MipiCsi::Ctrl>([](auto &s){ s. enable = 0; });

    LOGI << "Streaming stopped";
    m_Streaming = false;
}
//----------------------------------------------------------------------------
void GenX320::resetROI()
{
    // Set 1ast ROI as full image
    setROIWindow(0, 0, 320, 320, 0);

    // Apply 1 ROI
    applyROI(1, ROIMode::ROI);
}
//----------------------------------------------------------------------------
void GenX320::setEventFormat(EventFormat eventFormat)
{
    m_EventFormat = eventFormat;

    writeRegFields<EDF::Control>([this](auto &s)
                                  {
                                      s.format = m_EventFormat; 
                                      s.endianness = 0;   // little-endian
                                  });

    writeRegFields<EDF::PipelineControl>([](auto &s)
                                         {
                                             s.bypass = 0;
                                             s.enable = 1;
                                         });
    
    LOGI << "Event format set to " << m_EventFormat._to_string();
}
//----------------------------------------------------------------------------
void GenX320::waitBoot(int numRetries)
{
    using namespace std::chrono_literals;
    LOGI << "Waiting for sensor boot magic...";
    for(int i = 0; i < numRetries; i++) {
        const uint32_t magic = readReg<Mbx::Misc>();
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


    const uint32_t magic = readReg<Mbx::Misc>();
    if(magic != bootMagic) {
        // Diagnostic: read some other registers to see if digital core is alive
        const uint32_t cid = readReg<ChipId>();
        const uint32_t dsr = readReg<DigSoftReset>();
        const uint32_t scc = readReg<SysClkCtrl>();
        
        LOGE << std::hex << "GenX320 boot magic mismatch: got 0x" << magic << ", expected 0x" << bootMagic << ".";
        LOGE << std::hex << "Diagnostics: ChipID=" << cid << ", DigReset=0x" << dsr << ", SysClk=0x" << scc;
        throw std::runtime_error("Unable to boot camera");
    }

    LOGI << std::hex << "Boot magic OK (0x" << magic << ")";
}
//----------------------------------------------------------------------------
void GenX320::setROIWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, int index)
{
    const uint16_t xAddress = ROI::WinArray::address + (8 * index);
    const uint16_t yAddress = xAddress + 4;
    
    writeReg(xAddress, (x & 0x1FF) | (((x + w) & 0x1FF) << 16));
    writeReg(yAddress, (y & 0x1FF) | (((y + h) & 0x1FF) << 16));
}
//----------------------------------------------------------------------------
void GenX320::applyROI(uint32_t numROI, ROIMode roiMode)
{
    assert(numROI < 32);

    writeRegFields<ROI::Ctrl>([](auto &s)
                              {
                                s.px_roi_halt_programming = 0;
                              });
    writeRegFields<ROI::MasterCtrl>([numROI, roiMode](auto &s)
                                    {
                                        s.master_run = 1;
                                        s.master_mode = static_cast<uint32_t>(roiMode);
                                        s.win_nb = numROI;
                                    });

    // Wait until master isn't busy
    for(int i = 0; i < 50; i++) {
        if(!readRegFields<ROI::MasterCtrl>().master_busy) {
            break;
        }
    }

    // Wait until master is done
    for(int i = 0; i < 50; i++) {
        if(readRegFields<ROI::MasterCtrl>().master_done) {
            return;
        }
    }
    
    throw std::runtime_error("ROI apply timed out");
}
//----------------------------------------------------------------------------
uint32_t GenX320::readReg(uint16_t address)
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
void GenX320::writeReg(uint16_t address, uint32_t value, int numRetries)
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
//----------------------------------------------------------------------------
void GenX320::setRegBits(uint16_t address, uint32_t mask, int numRetries)
{
    const uint32_t val = readReg(address);
    writeReg(address, val | mask, numRetries);
}
//----------------------------------------------------------------------------
void GenX320::clearRegBits(uint16_t address, uint32_t mask, int numRetries)
{
    const uint32_t val = readReg(address);
    writeReg(address, val & ~mask, numRetries);
}

