#pragma once

// Standard C includes
#include <cstddef>
#include <cstdint>

// FeNN common includes
#include "fenn/common/fenn_common_export.h"
#include "fenn/common/uio.h"

namespace FeNN::Common
{
// Forward declarations
class DMABuffer;

//----------------------------------------------------------------------------
// FeNN::Common::DMAController
//----------------------------------------------------------------------------
class FENN_COMMON_EXPORT DMAController
{
public:
    DMAController(const std::string &uioName);
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Start write (from source DMA buffer to URAM)
    void startWrite(uint32_t destination, const DMABuffer &sourceBuffer, size_t sourceOffset, size_t size);
    
    //! Start read (from URAM to DMA buffer)
    void startRead(DMABuffer &destBuffer, size_t destOffset, uint32_t source, size_t size);

    // Wait for write channel to goto idle
    void waitForWriteComplete() const;

    // Wait for read channel to goto idle
    void waitForReadComplete() const;

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

        S2MM_SRC_ADDR   = 0x14, // S2MM source address (URAM)
        S2MM_DST_ADDR   = 0x18, // S2MM destination address (memory mapped domain)
        S2MM_COUNT      = 0x1C, // S2MM transfer size (bytes)
        S2MM_STATUS     = 0x20, // S2MM status register
        S2MM_CONTROL    = 0x24, // S2MM control register
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
    UIO m_RegisterUIO;
};
}   // namespace FeNN::Common