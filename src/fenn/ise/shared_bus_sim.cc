#include "fenn/ise/shared_bus_sim.h"


//----------------------------------------------------------------------------
// SharedBusSim
//----------------------------------------------------------------------------
std::pair<std::optional<uint32_t>, bool> SharedBusSim::synchronise(size_t routerIndex)
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
    
    // If the event that got sent was ours, update next router
    // **NOTE** this is an arbitrary choice of thread to update this
    if (readRouterIndex == routerIndex) {
        m_NextRouter = (readRouterIndex.value() + 1) % m_NumRouters;
    }

    // Wait until next router has definitely been updated
    m_Barrier.wait();

    // Return read data and whether the event that got send was 'ours'
    return std::make_pair(data, readRouterIndex == routerIndex);
}