#pragma once

// Standard C++ includes
#include <optional>
#include <type_traits>

// Standard C includes
#include <cassert>

//----------------------------------------------------------------------------
// FSM
//----------------------------------------------------------------------------
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
    template<typename TickHandler, typename EnterHandler, typename T = void>
    auto tick(EnterHandler handleEnter, TickHandler handleTick)
    {
        if constexpr(std::is_void_v<T>) {
            handleTick(m_CurrentState,
                    [this](S state)
                    { 
                        assert(!m_NextState);
                        m_NextState = state;
                    });
            handleEnterInternal(handleEnter);
        }
        else {
            const auto res = handleTick(m_CurrentState,
                    [this](S state)
                    { 
                        assert(!m_NextState);
                        m_NextState = state;
                    });
            handleEnterInternal(handleEnter);
            return res;
        }
    }
    
private:
    //------------------------------------------------------------------------
    // Private methods
    //------------------------------------------------------------------------
    template<typename EnterHandler>
    void handleEnterInternal(EnterHandler handleEnter)
    {
        // If a transition was queued during update
        if(m_NextState) {
            // Enter next state
            m_CurrentState = m_NextState.value();
            handleEnter(m_CurrentState);
            
            // Invalidate next state
            m_NextState = std::nullopt;
        }
    }

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    S m_CurrentState;
    std::optional<S> m_NextState;
};