#pragma once

// Standard C includes
#include <cstddef>
#include <cstdint>

// Third-party includes
#include "enum.h"


// FeNN common includes
#include "fenn/common/fenn_common_export.h"
#include "fenn/common/i2c_interface.h"
#include "fenn/common/uio.h"

namespace FeNN::Common
{
BETTER_ENUM(EventFormat, uint32_t, EVT2 = 0, EVT3 = 1, EVT21 = 2)
BETTER_ENUM(StreamingSource, uint32_t, PIXEL_ARRAY, RO_PATTERN, TS_PATTERN)
BETTER_ENUM(ROIMode, uint32_t, ROI=0, RONI=1)

// Forward declarations
class MIPICSI2Receiver;

//----------------------------------------------------------------------------
// FeNN::Common::GenX320
//----------------------------------------------------------------------------
class FENN_COMMON_EXPORT GenX320
{
public:
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    // Perform full power-on sequence on GenX320: reset -> surgical force boot -> detect -> init.
    GenX320(EventFormat eventFormat, const std::string &gpioUIOName, 
            MIPICSI2Receiver *mipiCSI2Receiver = nullptr, const std::string &i2cPath = "/dev/i2c-3", 
            int muxSlaveAddress = 0x74, int camSlaveAddress = 0x3C);
    
    //! Graceful power-off: stop streaming and reset.
    ~GenX320();
    
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void startStreaming(StreamingSource source = StreamingSource::PIXEL_ARRAY);
    void stopStreaming();

    template<typename B>
    void setBias(uint8_t value)
    {
        setBias(B::address, value, B::min, B::max);
    }

    template<typename B>
    void resetBias()
    {
        setBias(B::address, B::def, B::min, B::max);
    }

    void enableERC(double eventsPerSecond);
    void disableERC();
    
    //! Reset ROI to full frame
    void resetROI();

    void setEventFormat(EventFormat eventFormat);

private:
    //------------------------------------------------------------------------
    // Constants
    //------------------------------------------------------------------------
    static inline const uint32_t bootMagic = 0xCAFEBABE;
    static inline const uint32_t chipIDExpected = 0xB0602003;
    static inline const uint16_t ERCRefPeriodDefault = 100;

    //------------------------------------------------------------------------
    // Private API
    //------------------------------------------------------------------------
    // Poll for boot magic with retries.
    void waitBoot(int numRetries = 50);

    //! Set the ROI specified by inde
    void setROIWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, int index);

    //! Apply the configured ROI windows
    void applyROI(uint32_t numROI, ROIMode roiMode);

    void setBias(uint16_t address, uint8_t value, uint8_t min, uint8_t max);

    //! Read the 32-bit camera register specified by address
    uint32_t readReg(uint16_t address);

    //! Write value to the 32-bit camera register specified by address
    void writeReg(uint16_t address, uint32_t value, int numRetries = 3);

    //! Set bits indicated by *mask* in register at *address*
    void setRegBits(uint16_t address, uint32_t mask, int numRetries = 3);

    //! Clear bits indicated by *mask* in register  at *address*
    void clearRegBits(uint16_t address, uint32_t mask, int numRetries = 3);

    //
    template<typename S>
    uint32_t readReg()
    {
        return readReg(S::address);
    }

    template<typename S>
    void writeReg(uint32_t value, int numRetries = 3)
    {
        return writeReg(S::address, value, numRetries);
    }

    template<typename S>
    S readRegFields()
    {
        // Create anonymous union with struct and raw
        union 
        {
            S s;
            uint32_t r;
        } regView;

        // Read current value into raw
        regView.r = readReg(S::address);

        return regView.s;
    }

    template<typename S, typename F>
    void writeRegFields(F updateFields, int numRetries = 3)
    {
        // Check register struct is 32-bit
        static_assert(sizeof(S) <= 4, "Register structs must be less than 4 bytes");

        // Create anonymous union with struct and raw
        union 
        {
            S s;
            uint32_t r;
        } regView;

        // Read current value into raw
        regView.r = readReg(S::address);

        // Allow user to update struct fields
        updateFields(regView.s);

        // Write back new value
        writeReg(S::address, regView.r, numRetries);
    }

    //! Set bits indicated by *mask* in register S
    template<typename S>
    void setRegBits(uint32_t mask, int numRetries = 3)
    {
        setRegBits(S::address, mask, numRetries);
    }

    //! Clear bits indicated by *mask* in register S
    template<typename S>
    void clearRegBits(uint32_t mask, int numRetries = 3)
    {
        clearRegBits(S::address, mask, numRetries);
    }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    EventFormat m_EventFormat;
    I2CInterface m_MuxI2C;
    I2CInterface m_CamI2C;
    UIO m_GPIUIO;
    MIPICSI2Receiver *m_MIPICSI2Receiver;
    bool m_Streaming;
};
}