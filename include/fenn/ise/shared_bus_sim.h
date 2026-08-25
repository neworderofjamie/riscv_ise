#pragma once

// Standard C++ includes
#include <optional>
#include <vector>

// Standard C includes
#include <cstdint>

// Common includes
#include "common/barrier.h"

//----------------------------------------------------------------------------
// FeNN::ISE::SharedBusSim
//----------------------------------------------------------------------------
namespace FeNN::ISE
{
class SharedBusSim
{
public:
    SharedBusSim(size_t numRouters);

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Sets value a router should TRY and transmit
    void send(size_t routerIndex, std::optional<uint32_t> value);
    
    //! Read value of shared bus and updates round-robin logic
    //! **NOTE** blocks until all threads reach this point and read value
    std::pair<std::optional<uint32_t>, bool> synchronise(size_t routerIndex, bool lastTick = false);
   
    size_t getNumRouters() const{ return m_NumRouters; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! How many routers are connected to bus
    size_t m_NumRouters;

    //! Data each master is trying to transmit
    std::vector<std::optional<uint32_t>> m_SendData;

    //! Which entry in send data corresponds to each router ID
    std::vector<std::optional<size_t>> m_RouterMap;

    //! Next router to give a chance to send data
    size_t m_NextRouter;

    ::Common::Barrier m_Barrier;

    //! Mutex to protect m_RouterMap and m_NumRouters
    std::mutex m_RemovalMutex;
};
}   // namespace FeNN::ISE