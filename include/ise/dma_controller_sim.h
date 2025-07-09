#pragma once

// Standard C++ includes
#include <array>
#include <functional>
#include <optional>
#include <vector>

// ISE includes
#include "ise/fsm.h"

// Forward declarations
class VectorDataMemory;

//----------------------------------------------------------------------------
// DMAControllerSim
//----------------------------------------------------------------------------
class DMAControllerSim
{
public:
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    // DMA controller registers
    enum class Register : int
    {
        MM2S_SRC_ADDR,  // MM2S source address (memory mapped domain)
        MM2S_DST_ADDR,  // MM2S destination address (URAM)
        MM2S_COUNT,     // MM2S transfer size (bytes)
        MM2S_STATUS,    // MM2S status register
        MM2S_CONTROL,   // MM2S control register

        S2MM_SRC_ADDR,  // S2MM source address (URAM)
        S2MM_DST_ADDR,  // S2MM destination address (memory mapped domain)
        S2MM_COUNT,     // S2MM transfer size (bytes)
        S2MM_STATUS,    // S2MM status register
        S2MM_CONTROL,   // S2MM control register
        MAX,
    };

    DMAControllerSim(VectorDataMemory &vectorDataMemory, size_t bufferSize = 512 * 1024);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Start write (from source DMA buffer to URAM)
    void startWrite(uint32_t destination, uint32_t source, uint32_t size);
    
    //! Start read (from URAM to DMA buffer)
    void startRead(uint32_t destination, uint32_t source, uint32_t size);

    //! Wait for write channel to goto idle
    void waitForWriteComplete() const;

    //! Wait for read channel to goto idle
    void waitForReadComplete() const;

    //! Write to register
    void writeReg(Register reg, uint32_t val);

    //! Read from register
    uint32_t readReg(Register reg) const;

    //! Update state
    void tick();

    //! Get buffer data
    const uint8_t *getData() const{ return m_Data.data(); }
    uint8_t *getData(){ return m_Data.data(); }

private:
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
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

    enum class FSMState
    {
        IDLE,
        TRANSFERRING,
    };

    //------------------------------------------------------------------------
    // Transfer
    //------------------------------------------------------------------------
    struct Transfer
    {
        uint32_t source;
        uint32_t destination;
        uint32_t count;
    };

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    // Vector data memory to DMA to and from
    std::reference_wrapper<VectorDataMemory> m_VectorDataMemory;

    //! Buffer used to DMA to and from
    std::vector<uint8_t> m_Data;

    //! Active trans
    std::optional<Transfer> m_ActiveMM2S;
    std::optional<Transfer> m_ActiveS2MM;

    // Counters used to simulate DRAM latency
    int m_MM2SDelay;
    int m_S2MMDelay;

    // FSMs
    FSM<FSMState> m_MM2SFSM;
    FSM<FSMState> m_S2MMFSM;

    // Registers
    std::array<uint32_t, static_cast<size_t>(Register::MAX)> m_Registers;
};