#pragma once

// Standard C includes
#include <cstddef>
#include <cstdint>

// Common includes
#include "common/common_export.h"
#include "common/i2c_interface.h"
#include "common/uio.h"

//----------------------------------------------------------------------------
// GenX320
//----------------------------------------------------------------------------
class COMMON_EXPORT GenX320
{
public:
    GenX320(const std::string &gpioUIOName, const std::string &i2cPath = "/dev/i2c-3", 
            int muxSlaveAddress = 0x74, int camSlaveAddress = 0x3C);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void powerOn();

private:
    //------------------------------------------------------------------------
    // Constants
    //------------------------------------------------------------------------
    static inline const uint32_t bootMagic = 0xCAFEBABE;
    static inline const uint32_t chipIDExpected = 0xB0602003;

    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    // DMA controller registers
    enum class RegisterAddress : uint16_t
    {
        ROI_CONTROL                             = 0x0000,
        CHIP_ID                                 = 0X0014, 
        DIG_SOFT_RESET                          = 0X001C,
        RO_TD_CTRL                              = 0X002C,
        ROI_MASTER_CTRL                         = 0X0034, 
        ROI_MASTER_CHICKEN_BIT                  = 0X0044,
        SRAM_INITN                              = 0X00B8, 
        SRAM_PD1                                = 0X00C0, 
        SYS_CLK_CTRL                            = 0X0204,
        EVT_ICN_CLK_CTRL                        = 0X0210,
        PLL_CTRL                                = 0X0214, 
        IO_CTRL2                                = 0X0608,
        BGEN_CTRL                               = 0X1208,
        ERC_PIPELINE_CONTROL                    = 0X6000,
        ERC_AHVT_DROPPING_CONTROL               = 0X6014,
        ERC_TD_TARGET_EVENT_COUNT               = 0X602C,
        ERC_REF_PERIOD_FLAVOR                   = 0X6030,
        ERC_MONITORING_EVENT_CONTROL            = 0X6034,
        ERC_DELAY_FIFO_FLUSH_AND_BYPASS         = 0X60A0,
        ERC_RESET_TDROP_COUNTER_ON_MTAG_FIRST   = 0X60B4,
        EDF_PIPELINE_CONTROL                    = 0X7000, 
        EDF_CONTROL                             = 0X7044, 
        EDF_OUTPUT_INTERFACE_CONTROL            = 0X704C,
        EDF_EXTERNAL_OUTPUT_ADAPTER             = 0X7100, 
        CPI_PIPELINE_CONTROL                    = 0X8000, 
        
        RO_READOUT_CTRL                         = 0X9000, 
        RO_SELF_TEST_DATA                       = 0X901C,
        RO_TIME_BASE_CTRL                       = 0X9008,
        RO_LP_CTRL                              = 0X9028, 
        
        MIPI_CSI_CTRL                           = 0XB000,
        MIPI_CSI_FRAME_CTRL                     = 0XB010,
        MIPI_CSI_BL_FRAME                       = 0XB024, 
        MIPI_CSI_STAT_CTRL                      = 0XB080,
        MIPI_CSI_STAT_FRAME_CNT                 = 0XB084,
        MIPI_CSI_STAT_BYTE_CNT                  = 0XB088,
        MIPI_CSI_STAT_PAD_CNT                   = 0XB08C,
        MIPI_CSI_STAT_PKT_CNT                   = 0XB090,
        MIPI_CSI_STAT_INC_PKT_CNT               = 0XB094,
        MIPI_CSI_STAT_FRAME_PERIOD              = 0XB098,

        MBX_CPU_SOFT_RESET                      = 0XF004, 
        MBX_MISC                                = 0XF010, 
        TD_ROI_Y_ARRAY                          = 0X3000, 
    };

    //------------------------------------------------------------------------
    // Private API
    //------------------------------------------------------------------------
    uint32_t readCamRegister(RegisterAddress address);
    void writeCamRegister(RegisterAddress address, uint32_t value, int numRetries = 3);

    void waitBoot(int numRetries = 50);

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    I2CInterface m_MuxI2C;
    I2CInterface m_CamI2C;
    UIO m_GPIUIO;
};