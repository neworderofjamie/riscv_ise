#include "fenn/ise/shared_bus_sim.h"

// Standard C++ includes
#include <algorithm>
#include <numeric>

// Standard C includes
#include <cassert>

//----------------------------------------------------------------------------
// FeNN::ISE::SharedBusSim
//----------------------------------------------------------------------------
namespace FeNN::ISE
{
SharedBusSim::SharedBusSim(size_t numRouters)
:   m_NextRouter(0), m_Barrier(numRouters), m_RouterMap(numRouters)
{
    reset(numRouters);
}
//----------------------------------------------------------------------------
void SharedBusSim::send(size_t routerIndex, std::optional<uint32_t> value) 
{ 
    std::lock_guard<std::mutex> lock(m_RemovalMutex);

    m_SendData.at(m_RouterMap[routerIndex].value()) = value; 
}
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
    
    // If this is the last tick we want to make and our router has send it's data or there is no data to send
    if(lastTick) {
        // Check we're the last router
        // **TODO** more flexible data structure
        //assert(routerIndex == (m_NumRouters - 1));

        // Wait for barrier and remove ourselves from future synchronisation
        m_Barrier.waitAndDrop();

        {
            std::lock_guard<std::mutex> lock(m_RemovalMutex);

            // Get index of this router in m_SendData
            const size_t sendDataIndex = m_RouterMap[routerIndex].value();
            
            // Invalidate this router's router map entry now it is disconnected
            m_RouterMap[routerIndex] = std::nullopt;

            // If this entry wasn't already pointing at last send data slot,
            // point whichever router map points at m_SendData.size() - 1 and sendDataIndex
            const size_t lastSendDataSlot = (m_SendData.size() - 1);
            if(sendDataIndex != lastSendDataSlot) {
                auto swapRouter = std::find(m_RouterMap.begin(), m_RouterMap.end(), lastSendDataSlot);
                *swapRouter = sendDataIndex;

            }
    
            // (Arbitrarily) remove last element of m_SendData
            m_SendData.pop_back();

            // Decrement router count
            m_NumRouters--;
        }
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
void SharedBusSim::reset(size_t numRouters)
{
    // Reset router count
    m_NumRouters = numRouters;

    // Resize send data
    m_SendData.resize(numRouters, std::nullopt);

    // Generate initial router map
    std::iota(m_RouterMap.begin(), m_RouterMap.end(), 0);

    // Reset barrier
    m_Barrier.reset(numRouters);
}
}