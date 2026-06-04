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
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    enum EventFormat 
    {
         EVT2   = 0,
         EVT3   = 1,
         EVT21  = 2,
    };

    GenX320(EventFormat eventFormat, const std::string &gpioUIOName, 
            const std::string &i2cPath = "/dev/i2c-3", 
            int muxSlaveAddress = 0x74, int camSlaveAddress = 0x3C);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void powerOn();

    void setEventFormat(EventFormat eventFormat);

private:
    //------------------------------------------------------------------------
    // Constants
    //------------------------------------------------------------------------
    static inline const uint32_t bootMagic = 0xCAFEBABE;
    static inline const uint32_t chipIDExpected = 0xB0602003;

    //------------------------------------------------------------------------
    // Private API
    //------------------------------------------------------------------------
    void waitBoot(int numRetries = 50);

    uint32_t readCamRegister(uint16_t address);
    void writeCamRegister(uint16_t address, uint32_t value, int numRetries = 3);

    template<typename S>
    uint32_t readCamRegister()
    {
        return readCamRegister(S::address);
    }

    template<typename S>
    void writeCamRegister(uint32_t value, int numRetries = 3)
    {
        return writeCamRegister(S::address, value, numRetries);
    }

    template<typename S, typename F>
    void writeCamRegisterFields(F updateFields, int numRetries = 3)
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
        regView.r = readCamRegister(S::address);

        // Allow user to update struct fields
        updateFields(regView.s);

        // Write back new value
        writeCamRegister(S::address, regView.r, numRetries);
    }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    EventFormat m_EventFormat;
    I2CInterface m_MuxI2C;
    I2CInterface m_CamI2C;
    UIO m_GPIUIO;
};