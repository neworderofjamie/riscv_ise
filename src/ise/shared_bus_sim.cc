#include "ise/shared_bus_sim.h"

// Standard C includes
#include <cassert>

//----------------------------------------------------------------------------
// SharedBusSim
//----------------------------------------------------------------------------
std::pair<std::optional<uint32_t>, bool> SharedBusSim::synchronise(size_t routerIndex, bool lastTick)
{
    // Wait until all threads have written data
    m_Barrier.wait();

    // Loop through routers
    std::optional<uint32_t> data;
    std::optional<size_t> readRouterIndex;
    for (size_t i = 0; i < m_NumRouters; i++) {
        // If this router has data
        const size_t roundRobinIndex = (i + m_NextRouter) % m_NumRouters;
        data = m_SendData.at(roundRobinIndex);
        if (data) {
            // Set index of read master and stop searching
            readRouterIndex = roundRobinIndex;
            break;
        }
    }
    
    // If this is the last tick we want to make and our router has send it's data
    if(lastTick && readRouterIndex == routerIndex) {
        // Check we're the last router
        // **TODO** more flexible data structure
        assert(routerIndex == (m_NumRouters - 1));

        // Decrement router count
        m_NumRouters--;

        // Wait for barrier and remove ourselves from future synchronisation
        m_Barrier.waitAndDrop();
    }
    // Otherwise, just wait for barrier
    else {
        m_Barrier.wait();
    }

    // If the event that got sent was ours, update next router
    // **NOTE** this is an arbitrary choice of thread to update this
    if (readRouterIndex == routerIndex) {
        m_NextRouter = (readRouterIndex.value() + 1) % m_NumRouters;
    }

    // Return read data and whether the event that got send was 'ours'
    return std::make_pair(data, readRouterIndex == routerIndex);
}