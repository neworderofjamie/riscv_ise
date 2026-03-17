#pragma once

// Standard C includes
#include <cstddef>
#include <cstdint>

// Common includes
#include "common/common_export.h"
#include "common/uio.h"

//----------------------------------------------------------------------------
// AxisTkeepHandler
//----------------------------------------------------------------------------
class COMMON_EXPORT AxisTkeepHandler
{
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    // AXIS tkeep Handler registers
    enum class Register : int
    {
        VERSION = 0x0,      // IP version
        CONTROL = 0x4,      // Control register
        CONFIG  = 0x8,      // Configuration register
    };

    // Control register bits
    enum class ControlBits : uint32_t
    {
        ENABLE  = (1 << 0), // Enable the tkeep handler, default is 1. When disabled the input ready signal is 0.
        BYPASS  = (1 << 1), // Bypass the tkeep handler, default is 1: input stream directly connected to output stream.
        CLEAR   = (1 << 2), // Clear the content of all data registers.
    };

public:
    AxisTkeepHandler(const std::string &uioName);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void setEnabled(bool enable){ setControlBit(ControlBits::ENABLE, enable); }
    bool isEnabled() const{ return getControlBit(ControlBits::ENABLE); }

    void setBypased(bool bypass) { setControlBit(ControlBits::BYPASS, bypass); }
    bool isBypassed() const{ return getControlBit(ControlBits::BYPASS); }

    void setClear(bool clear) { setControlBit(ControlBits::CLEAR, clear); }

private:
    //------------------------------------------------------------------------
    // Private API
    //------------------------------------------------------------------------
    void writeReg(Register reg, uint32_t val);
    uint32_t readReg(Register reg) const;

    bool getControlBit(ControlBits controlBit) const;
    void setControlBit(ControlBits controlBit, bool set);

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    UIO m_RegisterUIO;
};