#include "ise/shared_bus_sim.h"


//----------------------------------------------------------------------------
// SharedBusSim
//----------------------------------------------------------------------------
void SharedBusSim::send(uint32_t value)
{
    // Try and acquire bus master mutex
    std::lock_guard<std::mutex> masterLock(m_MasterMutex);

    // Wait until all slaves are ready
    {
        std::unique_lock<std::mutex> signalLock(m_SignalMutex);
        m_SlaveMasterCV.wait(signalLock, [this]() { return m_SlaveReadyCount == m_NumSlaves; });
    }

    // Send data to slaves
    {
        std::lock_guard<std::mutex> signalLock(m_SignalMutex);
        
        // Copy value to bus and zero ready count
        m_SlaveReadyCount = 0;
        m_Data = value;

        // Notify all slaves that there is data on bus
        signalSlaves();
    }

    
    // Wait until all slaves have read bus value
    {
        std::unique_lock<std::mutex> signalLock(m_SignalMutex);
        m_SlaveMasterCV.wait(signalLock, [this]() { return m_SlaveReadyCount == m_NumSlaves; });

        // Clear bus and zero ready count
        m_Data = std::nullopt;
        m_SlaveReadyCount = 0;

        // Notify all slaves that there is NO data on bus
        signalSlaves();
    }
}
//----------------------------------------------------------------------------
std::optional<uint32_t> SharedBusSim::read(const std::atomic<bool> &shouldQuit)
{
    // Signal master that we're ready
    {
        std::lock_guard<std::mutex> signalLock(m_SignalMutex);
        m_SlaveReadyCount++;

        // Notify active master that read count has been updated
        m_SlaveMasterCV.notify_one();
    }

    // Wait until there is data on bus or we should quit
    std::unique_lock<std::mutex> signalLock(m_SignalMutex);
    m_MasterSlaveCV.wait(signalLock, [this, &shouldQuit]() { return m_Data.has_value() || shouldQuit; });

    // If there's data
    if(m_Data.has_value()) {
        // Stash/print received data and increment read counter
        const uint32_t value = m_Data.value();
        m_SlaveReadyCount++;

        // Notify active master that read count has been updated
        m_SlaveMasterCV.notify_one();

        // Wait until master clears bus
        m_MasterSlaveCV.wait(signalLock, [this]() { return !m_Data.has_value(); });
    
        return value;
    }
    // Otherwise, return nullopt
    else {
        return std::nullopt;
    }
}
//----------------------------------------------------------------------------
void SharedBusSim::signalSlaves()
{
    m_MasterSlaveCV.notify_all();
}