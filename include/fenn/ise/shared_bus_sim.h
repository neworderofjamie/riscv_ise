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
    SharedBusSim(size_t numRouters)
    :   m_NumRouters(numRouters), m_SendData(numRouters), m_NextRouter(0), m_Barrier(m_NumRouters)
    {}
    
    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    //! Sets value a router should TRY and transmit
    void send(size_t routerIndex, std::optional<uint32_t> value) { m_SendData.at(routerIndex) = value; }
    
    //! Read value of shared bus and updates round-robin logic
    //! **NOTE** blocks until all threads reach this point and read value
    std::pair<std::optional<uint32_t>, bool> synchronise(size_t routerIndex);
   
private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    //! How many routers are connected to bus
    size_t m_NumRouters;

    //! Data each master is trying to transmit
    std::vector<std::optional<uint32_t>> m_SendData;

    //! Next router to give a chance to send data
    size_t m_NextRouter;

    Barrier m_Barrier;
};
}   // namespace FeNN::ISE