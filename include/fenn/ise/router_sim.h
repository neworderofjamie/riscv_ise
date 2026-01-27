#pragma once

// Standard C++ includes
#include <array>
#include <functional>
#include <optional>

// Standard C includes
#include <cstdint>

// ISE includes
#include "fenn/ise/fsm.h"
#include "fenn/ise/ise_export.h"

// Forward declarations
class ScalarDataMemory;
class SharedBusSim;

//----------------------------------------------------------------------------
// RouterSim
//----------------------------------------------------------------------------
class ISE_EXPORT RouterSim
{
public:
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    // Router registers
    enum class Register : int
    {
        MASTER_EVENT_ID_BASE,   // Bits to OR with bitfield to get full event IDs
        MASTER_EVENT_BITFIELD,  // Register to write event bitfields to
        MASTER_SEND_BARRIER,    // Control register to send a barrier
        SLAVE_EVENT_ADDRESS,    // Current address slave writes addresses to
        SLAVE_BARRIER_COUNT,    // Number of barrier events received
        MAX,
    };

    RouterSim(SharedBusSim &sharedBus, ScalarDataMemory &spikeMemory, size_t routerIndex);

    // Update state every clock cycle
    void tick();

    //! Write to register
    void writeReg(Register reg, uint32_t val);

    //! Read from register
    uint32_t readReg(Register reg) const;

private:
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    enum class MasterFSMState
    {
        IDLE,
        EXTRACT_BIT,
        WAIT_SPIKE_SENT,
        WAIT_BARRIER_SENT,
    };

    //------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------
    void writeReceivedEvent(std::optional<uint32_t> data);

    //! Write to register
    void writeRegInternal(Register reg, uint32_t val);

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! Shared bus to communicate over
    std::reference_wrapper<SharedBusSim> m_SharedBus;

    //! Spike memory to write spikes to
    std::reference_wrapper<ScalarDataMemory> m_SpikeMemory;

    //! Index of this router on shared bus
    size_t m_RouterIndex;

    // FSMs
    FSM<MasterFSMState> m_MasterFSM;

    //! Registers
    std::array<uint32_t, static_cast<size_t>(Register::MAX)> m_Registers;

    //! Bitfield currently being decoded
    uint32_t m_CurrentSpikeBitfield;

    //!
    uint32_t m_CurrentEventIDBase;

    //! Spike router is currently waiting to write
    uint32_t m_CurrentSpikeID;
};