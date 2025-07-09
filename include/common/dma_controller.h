#pragma once

// Standard C includes
#include <cstddef>
#include <cstdint>

// Common includes
#include "common/common_export.h"

// Forward declarations
class DMABuffer;

//----------------------------------------------------------------------------
// DMAController
//----------------------------------------------------------------------------
class COMMON_EXPORT DMAController
{
public:
    DMAController(int memory, size_t baseAddress);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Start write (from source DMA buffer to URAM)
    void startWrite(uint32_t destination, const DMABuffer &sourceBuffer, size_t sourceOffset, size_t size);
    
    //! Start read (from URAM to DMA buffer)
    //void startRead(DMABuffer &destBuffer, size_t destOffset, uint32_t source, size_t size);

    // Wait for write channel to goto idle
    void waitForWriteComplete() const;

    // Wait for read channel to goto idle
    //void waitForReadComplete() const;

private:
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    // DMA controller registers
    enum class Register : int
    {
        MM2S_SRC_ADDR   = 0x0,  // MM2S source address (memory mapped domain)
        MM2S_DST_ADDR   = 0x4,  // MM2S destination address (URAM)
        MM2S_COUNT      = 0x8,  // MM2S transfer size (bytes)
        MM2S_STATUS     = 0xC,  // MM2S status register
        MM2S_CONTROL    = 0x10, // MM2S control register
    };

    // Status register bits
    enum class StatusBits : uint32_t
    {
        RUNNING         = (1 << 0),    // **TODO** remove
        STATE_IDLE      = (1 << 1), // DMA controller is in idle state
        STATE_START     = (1 << 2), // DMA controller is in start state
        STATE_TRANSFER  = (1 << 3), // DMA controller is transferring data
        ERROR_INTERNAL  = (1 << 4), // Data mover encountered an internal error
        ERROR_DECODE    = (1 << 5), // Data mover encountered an address decoding error
        ERROR_SLAVE     = (1 << 6), // Data mover received an error from slave
        TRANSFER_OK     = (1 << 7), // Data mover transfer succeeded
    };

    //------------------------------------------------------------------------
    // Private API
    //------------------------------------------------------------------------
    void writeReg(Register reg, uint32_t val);
    uint32_t readReg(Register reg) const;

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    uint32_t *m_Registers;
};
