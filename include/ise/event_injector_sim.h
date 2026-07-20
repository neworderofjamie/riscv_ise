#pragma once

// Standard C++ includes
#include <functional>

// Standard C includes
#include <cstdint>

// ISE includes
#include "ise/fsm.h"
#include "ise/ise_export.h"

// Forward declarations
class SharedBusSim;

//----------------------------------------------------------------------------
// RouterSim
//----------------------------------------------------------------------------
class ISE_EXPORT EventInjectorSim
{
public:
    EventInjectorSim(SharedBusSim &sharedBus, const std::vector<uint32_t> &data, size_t routerIndex);

    // Update state every clock cycle
    bool tick();

private:
    //------------------------------------------------------------------------
    // Enumerations
    //------------------------------------------------------------------------
    enum class FSMState
    {
        WAIT_OTHER_BARRIERS,
        WAIT_BARRIER_SENT,
        WAIT_SPIKE_SENT,
    };

    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    void handleBarrier(std::optional<uint32_t> data);

    bool areThereSpikesToSend() const;

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! Shared bus to communicate over
    std::reference_wrapper<SharedBusSim> m_SharedBus;

    // FSMs
    FSM<FSMState> m_FSM;

    std::vector<uint32_t> m_Data;

    //! Index of this router on shared bus
    size_t m_RouterIndex;

    size_t m_ReadPointer;
    
    uint32_t m_Timestep;

    uint32_t m_BarrierCount;
};