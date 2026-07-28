#pragma once

// Standard C includes
#include <cstddef>
#include <cstdint>

// Common includes
#include "common/common_export.h"
#include "common/uio.h"


//----------------------------------------------------------------------------
// MIPICSI2Receiver
//----------------------------------------------------------------------------
class COMMON_EXPORT MIPICSI2Receiver
{
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    // MIPI CSI-2 RX Controller registers
    enum class Register : int
    {
        CORE_CONFIG     = 0x0,  // Core configuration options 
        PROTOCOL_CONFIG = 0x4,  // Protocol configuration options 
        CORE_STATUS     = 0x10, // Configuration register
        INT_STATUS      = 0x24, // Interrupt Status (CRC, Frame Sync, etc.)
    };

    // Core config register bits
    enum class CoreConfigBits : uint32_t
    {
        CORE_ENABLE = (1 << 0), // 1: Enables the core to receive and process packets 0: Disables the core for operation 
        SOFT_RESET  = (1 << 1), // 1: Resets the core 0: Takes core out of soft reset 
    };

    // Core status register bits
    enum class CoreStatusBits : uint32_t
    {
        RESET_DISABLE_IN_PROCESS    = (1 << 0), // Set to 1 by the core to indicate that internal soft reset/core disable activities are in progress 
        STREAM_LINE_BUFFER_FULL     = (1 << 1), // Indicates the current status of line buffer full condition 
        SHORT_PACKET_FIFO_NOT_EMPTY = (1 << 2), // FIFO not empty: Indicates the current status of short packet FIFO not empty condition 
        SHORT_PACKET_FIFO_FULL      = (1 << 3), // Indicates the current status of short packet FIFO full condition 
    };

public:
    MIPICSI2Receiver(const std::string &uioName);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void setCoreEnabled(bool enable){ setRegBit(Register::CORE_CONFIG, CoreConfigBits::CORE_ENABLE, enable); }
    void setLanes(uint32_t activeLanes, uint32_t maxLanes);

    void setSoftReset(bool softReset){ setRegBit(Register::CORE_CONFIG, CoreConfigBits::SOFT_RESET, softReset); }

    void setInterruptStatus(uint32_t mask){ writeReg(Register::INT_STATUS, mask); }

    bool isResetDisableInProgress() const{ return getRegBit(Register::CORE_STATUS, CoreStatusBits::RESET_DISABLE_IN_PROCESS); }
    bool isStreamLineBufferFull() const{ return getRegBit(Register::CORE_STATUS, CoreStatusBits::STREAM_LINE_BUFFER_FULL); }
    bool isShortPacketFIFONotempty() const{ return getRegBit(Register::CORE_STATUS, CoreStatusBits::SHORT_PACKET_FIFO_NOT_EMPTY); }
    bool isShortPacketFIFOFull() const{ return getRegBit(Register::CORE_STATUS, CoreStatusBits::SHORT_PACKET_FIFO_FULL); }
    
    bool isCoreEnabled() const{ return getRegBit(Register::CORE_CONFIG, CoreConfigBits::CORE_ENABLE); }
    uint32_t getMaxLanes() const;
    uint32_t getActiveLanes() const;

    uint32_t getPacketCount() const;

private:
    //------------------------------------------------------------------------
    // Private API
    //------------------------------------------------------------------------
    void writeReg(Register reg, uint32_t val);
    uint32_t readReg(Register reg) const;

    //! Get a typed bit from an enumerated register
    template<typename B>
    bool getRegBit(Register reg, B bit) const
    {
        return ((readReg(reg) & static_cast<uint32_t>(bit)) != 0);
    }
    
    //! Set a typed bit in an enumerated register
    template<typename B>
    void setRegBit(Register reg, B bit, bool set)
    {
        // Read old value
        uint32_t oldVal = readReg(reg);

        // Either set or toggle bit in value
        if(set) {
            oldVal |= static_cast<uint32_t>(bit);
        }
        else {
            oldVal &= ~static_cast<uint32_t>(bit);
        }

        // Write updated value
        writeReg(reg, oldVal);
    }
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    UIO m_RegisterUIO;
};