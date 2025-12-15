#pragma once

// Standard C++ includes
#include <array>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>

// Standard C includes
#include <cstdint>

// Forward declarations
class ScalarDataMemory;
class SharedBusSim;

//----------------------------------------------------------------------------
// RouterSim
//----------------------------------------------------------------------------
class RouterSim
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
        SLAVE_EVENT_ADDRESS,    // Current address slave writes addresses to
        SLAVE_BARRIER_ADDRESS,  // Address slave writes barrier to
        MAX,
    };

    RouterSim(SharedBusSim &sharedBus, ScalarDataMemory &spikeMemory);
    ~RouterSim();

    // Update state every clock cycle
    void tick();

    //! Write to register
    void writeReg(Register reg, uint32_t val);

    //! Read from register
    uint32_t readReg(Register reg) const;

private:
    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    void masterThreadFunc();
    
    //! Thread functin
    void slaveThreadFunc();

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! Shared bus to communicate over
    std::reference_wrapper<SharedBusSim> m_SharedBus;

    //! Spike memory to write spikes to
    std::reference_wrapper<ScalarDataMemory> m_SpikeMemory;

    //! Master thread - handles writing data from spike queue to simulated bus
    std::thread m_MasterThread;

    //! Slave thread - handles reading packets from simulated bus
    std::thread m_SlaveThread;

    //! Signal used to kill master and slave threads
    std::atomic<bool> m_ShouldQuit;

    //! Queue of bases and bitfields to send via master interface
    std::deque<std::pair<uint32_t, uint32_t>> m_MasterSpikeQueue;

    //! Mutex guarding master spike queue
    std::mutex m_MasterSpikeQueueMutex;

    //! Condition variable used to signal master thread when there are spikes
    std::condition_variable m_MasterSpikeQueueCV;

    //! Queue of spikes received via slave interface to write to BRAM
    std::deque<uint32_t> m_SlaveSpikeQueue;

    //! Mutex guarding slave spike queue
    std::mutex m_SlaveSpikeQueueMutex;

    // Registers
    std::array<uint32_t, static_cast<size_t>(Register::MAX)> m_Registers;
};