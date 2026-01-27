#pragma once

// Standard C++ includes
#include <optional>

// Standard C includes
#include <cassert>

//----------------------------------------------------------------------------
// FeNN::ISE::FSM
//----------------------------------------------------------------------------
namespace FeNN::ISE
{
template<typename S>
class FSM
{
public: 
    FSM(S initialState)
    :   m_CurrentState(initialState)
    {
    }

    //------------------------------------------------------------------------
    // Public API
    //------------------------------------------------------------------------
    template<typename TickHandler, typename EnterHandler>
    void tick(EnterHandler handleEnter, TickHandler handleTick)
    {
        // Tick state
        handleTick(m_CurrentState,
                   [this](S state)
                   { 
                       assert(!m_NextState);
                       m_NextState = state;
                   });
        
        // If a transition was queued during update
        if(m_NextState) {
            // Enter next state
            m_CurrentState = m_NextState.value();
            handleEnter(m_CurrentState);
            
            // Invalidate next state
            m_NextState = std::nullopt;
        }
    }
    
private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    S m_CurrentState;
    std::optional<S> m_NextState;
};
}   // namespace FeNN::ISE