#include "ise/router_sim.h"

// PLOG includes
#include <plog/Log.h>

// Common includes
#include "common/utils.h"

// ISE includes
#include "ise/riscv.h"
#include "ise/shared_bus_sim.h"

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
RouterSim::RouterSim(SharedBusSim &sharedBus, ScalarDataMemory &spikeMemory)
:   m_SharedBus(sharedBus), m_SpikeMemory(spikeMemory), m_ShouldQuit(false), m_Registers{0}
{
}
//----------------------------------------------------------------------------
RouterSim::~RouterSim()
{
    // Set flag to kill worker threads
    m_ShouldQuit = true;

    // Join master thread
    if(m_MasterThread.joinable()) {
        m_MasterThread.join();
    }

    // Join slave thread
    if(m_SlaveThread.joinable()) {
        m_SlaveThread.join();
    }
}
//----------------------------------------------------------------------------
void RouterSim::tick()
{
    // If a event bitfield has been written to the register
    if(readReg(Register::MASTER_EVENT_BITFIELD) != 0) {
        // Acquire master spike queue mutex
        std::lock_guard<std::mutex> lock(m_MasterSpikeQueueMutex);

        // Add current bitfield and ID base to master queue
        m_MasterSpikeQueue.emplace_back(std::make_pair(readReg(Register::MASTER_EVENT_BITFIELD),
                                                       readReg(Register::MASTER_EVENT_ID_BASE)));

        // Notify master thread
        m_MasterSpikeQueueCV.notify_one();

        // Zero register
        writeReg(Register::MASTER_EVENT_BITFIELD, 0);
    }
    // Otherwise, if a barrier should be triggered
    else if(readReg(Register::MASTER_SEND_BARRIER) != 0) {
        // Acquire master spike queue mutex
        std::lock_guard<std::mutex> lock(m_MasterSpikeQueueMutex);

        // Add token to queue
        m_MasterSpikeQueue.emplace_back(std::nullopt);

        // Notify master thread
        m_MasterSpikeQueueCV.notify_one();

        // Zero register
        writeReg(Register::MASTER_SEND_BARRIER, 0);
    }
    {
        // Acquire slave spike queue mutex
        std::lock_guard<std::mutex> lock(m_SlaveSpikeQueueMutex);

        // If there are any spikes in the slave spike queue
        uint32_t &address = m_Registers[static_cast<int>(Register::SLAVE_EVENT_ADDRESS)];
        if(!m_SlaveSpikeQueue.empty()) {
            // If ID is special barrier ID, increment barrier
            if(m_SlaveSpikeQueue.front() == barrierEventID) {
                m_Registers[static_cast<int>(Register::SLAVE_BARRIER_COUNT)]++;
            }
            // Otherwise, write spike at front of queue to memory and increment address
            else {
                m_SpikeMemory.get().write32(address, m_SlaveSpikeQueue.front());
                address += 4;
            }
            
            // Pop queue and increment address
            m_SlaveSpikeQueue.pop_front();
        }
    }
}
//----------------------------------------------------------------------------
void RouterSim::writeReg(Register reg, uint32_t val)
{
    m_Registers[static_cast<int>(reg)] = val;
}
//----------------------------------------------------------------------------
uint32_t RouterSim::readReg(Register reg) const
{
    return m_Registers[static_cast<int>(reg)];
}
//------------------------------------------------------------------------
void RouterSim::masterThreadFunc()
{
    // While we should keep running
    while(!m_ShouldQuit) {
        decltype(m_MasterSpikeQueue)::value_type spike;
        {
            // Wait until there are events in master spike queue or we should quit
            std::unique_lock<std::mutex> masterLock(m_MasterSpikeQueueMutex);
            m_MasterSpikeQueueCV.wait(masterLock, [this](){ return !m_MasterSpikeQueue.empty() || m_ShouldQuit; });

            // Exit thread loop if we should quit
            if(m_ShouldQuit) {
                break;
            }

            // Read and pop
            spike = m_MasterSpikeQueue.front();
            m_MasterSpikeQueue.pop_front();
        }

        // If event is a spike not a barrier
        if(spike) {
            // While there are bits in bitfield to process
            uint32_t spikeID = 0;
            do {
                // Count trailing zeros
                const uint32_t numTZ = ctz(spike.value().first);
            
                // Add number of trailing zeros to spike ID
                spikeID += numTZ;

                // OR spike ID with base
                m_SharedBus.get().send(spike.value().second | spikeID);

                // Increment spike ID to skip over spike
                spikeID++;

                // Shift off bits
                spike.value().first >>= (numTZ + 1);
            } while(spike.value().first != 0);
        }
        // Otherwise, if it's a barrier, send barrier event ID
        else {
            m_SharedBus.get().send(barrierEventID);
        }
    }
}
//------------------------------------------------------------------------
void RouterSim::slaveThreadFunc()
{
    // While we should keep running
    while(!m_ShouldQuit) {
        // Read spike from shared bus
        const auto spike = m_SharedBus.get().read(m_ShouldQuit);
        
        // Exit thread loop if we should quit
        if(m_ShouldQuit) {
            break;
        }

        // Acquire slave spike queue mutex and add spike to end of queue
        std::lock_guard<std::mutex> lock(m_SlaveSpikeQueueMutex);
        m_SlaveSpikeQueue.push_back(spike.value());
    }
}
