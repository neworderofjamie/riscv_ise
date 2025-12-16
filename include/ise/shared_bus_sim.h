#pragma once

// Standard C++ includes
#include <atomic>
#include <condition_variable>
#include <optional>
#include <thread>

// Standard C includes
#include <cstdint>


//----------------------------------------------------------------------------
// SharedBusSim
//----------------------------------------------------------------------------
class SharedBusSim
{
public:
    SharedBusSim(uint32_t numSlaves)
    :   m_NumSlaves(numSlaves), m_SlaveReadyCount(0)
    {}
    
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Called by master to broadcast value to all slaves
    //! Blocks until all slaves have received
    void send(uint32_t value);
    
    //! Called by slave to read value off bus
    //! Blocks until there is a value to read or quit set
    std::optional<uint32_t> read(const std::atomic<bool> &shouldQuit);
    
    void signalSlaves();

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! How many slaves are connected to bus
    uint32_t m_NumSlaves;
    
    //! Mutex, locked by active master for duration of bus write
    std::mutex m_MasterMutex;
    
    //! Mutex used with conditional variables for signalling
    std::mutex m_SignalMutex;
    
    //! Condition variable used by active master to signal slaves that there is data on the bus
    std::condition_variable m_MasterSlaveCV;

    //! Condition variable used by slaves to signal active master that they have read data
    std::condition_variable m_SlaveMasterCV;
    
    //! Data currently on shared bus
    std::optional<uint32_t> m_Data;
    
    //! How many slaves are ready? Used for counting that all slaves are
    //! ready to receive data and for counting that they all have!
    uint32_t m_SlaveReadyCount;
};