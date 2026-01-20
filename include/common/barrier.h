#pragma once

// Standard C++ includes
#include <condition_variable>
#include <thread>

// Standard C includes
#include <cassert>

//----------------------------------------------------------------------------
// Barrier
//----------------------------------------------------------------------------
//! Copy-paste of boost::barrier to use std::thread and std::condition_variable
class Barrier
{
public:
    explicit Barrier(unsigned int count) 
    :   m_Count(count), m_Generation(0), m_ResetCount(count)
    {
        assert(m_ResetCount != 0);
    }
    Barrier(const Barrier&) = delete;

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    void wait()
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        const unsigned int gen = m_Generation;

        if (--m_Count == 0) {
            m_Generation++;
            m_Count = m_ResetCount;
            lock.unlock();
            m_Cond.notify_all();
        }

        m_Cond.wait(lock, [gen, this](){ return gen != m_Generation; });
    }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    std::mutex m_Mutex;
    std::condition_variable m_Cond;
    unsigned int m_Count;
    unsigned int m_Generation;
    unsigned int m_ResetCount;
};