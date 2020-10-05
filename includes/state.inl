//
// Created by napolus on 7/24/20.
//

#include "stateManager.hpp"

template<typename STATE_ID>
void State<STATE_ID>::requestPushState(STATE_ID stateId)
{
    m_state_manager.pushState(stateId);
}

template<typename STATE_ID>
void State<STATE_ID>::requestPopState()
{
    m_state_manager.popState();
}

template<typename STATE_ID>
void State<STATE_ID>::requestClearState()
{
    m_state_manager.clearState();
}