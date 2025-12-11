#include "ise/shared_bus_sim.h"


//----------------------------------------------------------------------------
// SharedBusSim
//----------------------------------------------------------------------------
void SharedBusSim::send(uint32_t value)
{
    // Try and acquire bus master mutex
    std::lock_guard<std::mutex> masterLock(m_MasterMutex);

    {
        // Zero read count
        std::lock_guard<std::mutex> signalLock(m_SignalMutex);
        m_ReadCount = 0;

        // Copy value to bus
        m_Data = value;

        // Notify all slaves that there is data on bus
        m_MasterSlaveCV.notify_all();
    }

    
    // Wait until all slaves have read bus value
    {
        std::unique_lock<std::mutex> signalLock(m_SignalMutex);
        m_SlaveMasterCV.wait(signalLock, [this]() { return m_ReadCount == m_NumSlaves; });

        // Clear bus
        m_Data = std::nullopt;

        // Notify all slaves that there is NO data on bus
        m_MasterSlaveCV.notify_all();
    }
}
//----------------------------------------------------------------------------
uint32_t SharedBusSim::read()
{
    // Wait until there is data on bus
    std::unique_lock<std::mutex> signalLock(m_SignalMutex);
    m_MasterSlaveCV.wait(signalLock, [this]() { return m_Data.has_value(); });

    // Stash/print received data and increment read counter
    const uint32_t value = m_Data.value();
    m_ReadCount++;

    // Notify active master that read count has been updated
    m_SlaveMasterCV.notify_one();

    // Wait until master clears bus
    m_MasterSlaveCV.wait(signalLock, [this]() { return !m_Data.has_value(); });
    
    return value;
}
