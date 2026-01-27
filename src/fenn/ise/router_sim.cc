#include "fenn/ise/router_sim.h"

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/utils.h"

// ISE includes
#include "fenn/ise/riscv.h"
#include "fenn/ise/shared_bus_sim.h"

//----------------------------------------------------------------------------
// Anonymous namespace
//----------------------------------------------------------------------------
namespace
{
constexpr uint32_t barrierEventID = 0xFFFFFFFFull;
}

//----------------------------------------------------------------------------
// RouterSim
//----------------------------------------------------------------------------
RouterSim::RouterSim(SharedBusSim &sharedBus, ScalarDataMemory &spikeMemory, size_t routerIndex)
:   m_SharedBus(sharedBus), m_SpikeMemory(spikeMemory), m_RouterIndex(routerIndex), m_Registers{0},
    m_MasterFSM(MasterFSMState::IDLE), m_CurrentSpikeBitfield(0), m_CurrentEventIDBase(0), m_CurrentSpikeID(0)
{
}
//----------------------------------------------------------------------------
void RouterSim::tick()
{
    // Tick MM2S FSM
    m_MasterFSM.tick(
        // Enter
        [this](auto)
        {
        },
        // Tick
        [this](auto state, auto transition)
        {
            if (state == MasterFSMState::IDLE) {
                // Put no data on the bus
                m_SharedBus.get().send(m_RouterIndex, std::nullopt);

                // Synchronise with other routers and write any received events to memory
                writeReceivedEvent(m_SharedBus.get().synchronise(m_RouterIndex).first);

                // If a event bitfield has been written to the register
                if (readReg(Register::MASTER_EVENT_BITFIELD) != 0) {
                    LOGV << "Serialising bitfield " << readReg(Register::MASTER_EVENT_BITFIELD);

                    // Copy bitfield and event ID base from CSRs; and reset spike ID
                    m_CurrentSpikeBitfield = readReg(Register::MASTER_EVENT_BITFIELD);
                    m_CurrentEventIDBase = readReg(Register::MASTER_EVENT_ID_BASE);
                    m_CurrentSpikeID = 0;

                    // Start extracting first bit
                    transition(MasterFSMState::EXTRACT_BIT);

                    // Zero register
                    writeRegInternal(Register::MASTER_EVENT_BITFIELD, 0);
                }
                // Otherwise, if a barrier should be triggered
                // **NOTE** events have higher priority as barrier should come after all pending events
                else if (readReg(Register::MASTER_SEND_BARRIER) != 0) {
                    LOGV << "Sending barrier";

                    // Start trying to send barrier
                    transition(MasterFSMState::WAIT_BARRIER_SENT);

                    // Zero register
                    writeRegInternal(Register::MASTER_SEND_BARRIER, 0);
                }
            }
            else if (state == MasterFSMState::EXTRACT_BIT) {
                // Put no data on the bus
                m_SharedBus.get().send(m_RouterIndex, std::nullopt);

                // Synchronise with other routers and write any received events to memory
                writeReceivedEvent(m_SharedBus.get().synchronise(m_RouterIndex).first);

                // Count trailing zeros
                const uint32_t numTZ = ctz(m_CurrentSpikeBitfield);

                // Add number of trailing zeros to spike ID
                m_CurrentSpikeID += numTZ;

                // Shift off bits
                m_CurrentSpikeBitfield = (numTZ == 31) ? 0 : (m_CurrentSpikeBitfield >> (numTZ + 1));

                // Wait for spike to be sent
                transition(MasterFSMState::WAIT_SPIKE_SENT);
            }
            else if (state == MasterFSMState::WAIT_SPIKE_SENT) {
                // OR current spike ID with base and put on the bus
                m_SharedBus.get().send(m_RouterIndex, m_CurrentSpikeID | m_CurrentEventIDBase);

                // Synchronise with other routers
                const auto syncResult = m_SharedBus.get().synchronise(m_RouterIndex);

                // Synchronise with bus and write any received events to memory
                writeReceivedEvent(syncResult.first);

                // If OUR event was sent successfully
                if (syncResult.second) {
                    // If no spikes left in bitfield, transition to idle
                    if (m_CurrentSpikeBitfield == 0) {
                        transition(MasterFSMState::IDLE);
                    }
                    // Otherwise
                    else {
                        // Increment spike ID to skip over spike
                        m_CurrentSpikeID++;

                        // Send next spike
                        transition(MasterFSMState::EXTRACT_BIT);
                    }
                }
            }
            else if (state == MasterFSMState::WAIT_BARRIER_SENT) {
                // Put current spike ID on the bus
                m_SharedBus.get().send(m_RouterIndex, barrierEventID);

                // Synchronise with other routers
                const auto syncResult = m_SharedBus.get().synchronise(m_RouterIndex);

                // Synchronise with bus and write any received events to memory
                writeReceivedEvent(syncResult.first);

                // If OUR barrier was sent successfully, go back to idle
                if (syncResult.second) {
                    transition(MasterFSMState::IDLE);
                }
            }
        });
}
//----------------------------------------------------------------------------
void RouterSim::writeReg(Register reg, uint32_t val)
{
    // Check for event and barrier dropping
    if (reg == Register::MASTER_EVENT_BITFIELD && readReg(Register::MASTER_EVENT_BITFIELD) != 0) {
        LOGW << "Writing bitfield when previous bitfield not processed - events will be dropped!";
    }
    else if (reg == Register::MASTER_SEND_BARRIER && readReg(Register::MASTER_SEND_BARRIER) != 0) {
        LOGW << "Writing barrier when previous barrier not processed - deadlock immiment";
    }
    writeRegInternal(reg, val);
}
//----------------------------------------------------------------------------
uint32_t RouterSim::readReg(Register reg) const
{
    return m_Registers[static_cast<int>(reg)];
}
//------------------------------------------------------------------------
void RouterSim::writeReceivedEvent(std::optional<uint32_t> data)
{
    if (data) {
        // If ID is special barrier ID, increment barrier
        if (data.value() == barrierEventID) {
            m_Registers[static_cast<int>(Register::SLAVE_BARRIER_COUNT)]++;
            PLOGV << "Incremented barrier " << m_Registers[static_cast<int>(Register::SLAVE_BARRIER_COUNT)];
        }
        // Otherwise, write spike at front of queue to memory and increment address
        else {
            uint32_t& address = m_Registers[static_cast<int>(Register::SLAVE_EVENT_ADDRESS)];
            m_SpikeMemory.get().write32(address, data.value());
            PLOGV << "Writing event " << data.value() << " to " << address;
            address += 4;
        }
    }
}
//----------------------------------------------------------------------------
void RouterSim::writeRegInternal(Register reg, uint32_t val)
{
    m_Registers[static_cast<int>(reg)] = val;
}