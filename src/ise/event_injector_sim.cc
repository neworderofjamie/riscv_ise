#include "ise/event_injector_sim.h"

// PLOG includes
#include <plog/Log.h>

// ISE includes
#include "ise/shared_bus_sim.h"

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
constexpr uint32_t barrierEventID = 0xFFFFFFFFu;
constexpr uint32_t timestampBit = (1u << 31u);
}

//----------------------------------------------------------------------------
// RouterSim
//----------------------------------------------------------------------------
EventInjectorSim::EventInjectorSim(SharedBusSim &sharedBus, const std::vector<uint32_t> &data, size_t routerIndex)
:   m_SharedBus(sharedBus), m_FSM(FSMState::IDLE), m_Data(data), 
    m_RouterIndex(routerIndex), m_ReadPointer(0), m_Timestep(0), m_BarrierCount(0)
{
}
//----------------------------------------------------------------------------
bool EventInjectorSim::tick()
{
    // Tick MM2S FSM
    m_FSM.tick(
        // Enter
        [this](auto)
        {
        },
        // Tick
        [this](auto state, auto transition)
        {
            if (state == FSMState::IDLE) {
                // Put no data on the bus
                m_SharedBus.get().send(m_RouterIndex, std::nullopt);

                // Synchronise with other routers and handle any barriers which are received
                const bool newTimestep = handleBarrier(m_SharedBus.get().synchronise(m_RouterIndex).first);

                // If there are spikes to send this timestep
                if (areThereSpikesToSend()) {
                    // Advance read pointer over timestamp
                    m_ReadPointer++;

                    // Transition to sending spikes
                    transition(FSMState::WAIT_SPIKE_SENT);
                }
                // Otherwise, if a new timestep has begun, transition to sending a barrier
                else if (newTimestep) {
                    transition(FSMState::WAIT_BARRIER_SENT);
                }
            }
            else if (state == FSMState::WAIT_SPIKE_SENT) {
                // Put current spike data on bus
                m_SharedBus.get().send(m_RouterIndex, m_Data[m_ReadPointer]);

                // Synchronise with other routers
                const auto syncResult = m_SharedBus.get().synchronise(m_RouterIndex);

                // Synchronise with bus and write any received events to memory
                handleBarrier(syncResult.first);

                // If OUR event was sent successfully
                if (syncResult.second) {
                    // Advance read pointer
                    m_ReadPointer++;
                    
                    // If we're out of data or the next entry is a timestamp
                    if (m_ReadPointer >= m_Data.size() || (m_Data[m_ReadPointer] & timestampBit) == 1) {
                        transition(FSMState::WAIT_BARRIER_SENT);
                    }
                }
            }
            else if (state == FSMState::WAIT_BARRIER_SENT) {
                // Put current spike ID on the bus
                m_SharedBus.get().send(m_RouterIndex, barrierEventID);

                // Synchronise with other routers
                const auto syncResult = m_SharedBus.get().synchronise(m_RouterIndex);

                // Synchronise with bus and write any received events to memory
                handleBarrier(syncResult.first);

                // If OUR barrier was sent successfully, go back to idle
                if (syncResult.second) {
                    transition(FSMState::IDLE);
                }
            }
        });

    return (m_ReadPointer < m_Data.size());
}
//----------------------------------------------------------------------------
bool EventInjectorSim::handleBarrier(std::optional<uint32_t> data)
{
    if (data) {
        // If ID is special barrier ID
        if (data.value() == barrierEventID) {
            // Increment barrier
            m_BarrierCount++;
            PLOGV << "Incremented barrier: " << m_BarrierCount;

            // If all routers have sent barrier
            if (m_BarrierCount == m_SharedBus.get().getNumRouters()) {
                m_Timestep++;
                m_BarrierCount = 0;
                PLOGV << "Incremented timestep: " << m_Timestep;
                return true;
            }
        }
    }

    return false;
}
//----------------------------------------------------------------------------
bool EventInjectorSim::areThereSpikesToSend() const
{
    // If there is data to read
    if (m_ReadPointer < m_Data.size()) {
        // Read data and assert that it is a timestamp
        const uint32_t data = m_Data[m_ReadPointer];
        assert((data & timestampBit) == 1);

        // If timestep matches current timestep
        const uint32_t timestep = (data & ~timestampBit);
        if (timestep == m_Timestep) {
            return true;
        }
    }

    return false;
}