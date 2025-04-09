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
    DMAController(int memory, size_t baseAddress, size_t destRegisterBaseAddress);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Start write (from source DMA buffer to URAM)
    void startWrite(uint32_t destination, const DMABuffer &sourceBuffer, size_t sourceOffset, size_t size);
    
    //! Start read (from URAM to DMA buffer)
    //void startRead(DMABuffer &destBuffer, size_t destOffset, uint32_t source, size_t size);

    //! Reset DMA controller
    void reset();
    
    // Is write channel idle?
    bool isWriteIdle() const;

    // Is read channel idle?
    //bool isReadIdle() const;

    // Wait for write channel to goto idle
    void waitForWriteComplete() const;

    // Wait for read channel to goto idle
    //void waitForReadComplete() const;

private:
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    // Xilinx DMA controller registers
    enum class Register : int
    {
        MM2S_DMACR = 0x0,       // Write control register
        MM2S_DMASR = 0x4,       // Write status register
        MM2S_SA = 0x18,         // Lower 32-bits of write source address
        MM2S_SA_MSB = 0x1C,    // Upper 32-bits of write source address
        MM2S_LENGTH = 0x28,     // Write length (bytes)

        S2MM_DMACR = 0x30,      // Read control register
        S2MM_DMASR = 0x34,      // Read status register
        S2MM_DA = 0x48,         // Lower 32-bits of read source address
        S2MM_DA_MSB = 0x4C,    // Upper 32-bits of read source address
        S2MM_LENGTH = 0x58,     // Read length (bytes)
    };

    //------------------------------------------------------------------------
    // Private API
    //------------------------------------------------------------------------
    void writeReg(Register reg, uint32_t val);
    uint32_t readReg(Register reg) const;

    void writeURAMDestinationAddress(uint32_t val);

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    uint32_t *m_Registers;
    uint32_t *m_DestAddressRegister;
};
