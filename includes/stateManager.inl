//
// Created by napolus on 7/24/20.
//

#include <cassert>

#include "state.hpp"

template<typename STATE_ID>
template <typename T,typename ...ARGS>
void StateManager<STATE_ID>::registerState(STATE_ID stateId,ARGS&& ...args)
{
    m_factories[stateId] =[&,this]()->typename State<STATE_ID>::Ptr
                            {return std::make_unique<T>(*this,std::forward<ARGS>(args)...);};
}

template<typename STATE_ID>
void StateManager<STATE_ID>::processEvent()
{
    for(auto &state:m_stateStack)
        if(!state->processEvent())
            break;

}

template<typename STATE_ID>
void StateManager<STATE_ID>::update(sf::Time deltatime)
{
    applyChange();
    for(auto &state:m_stateStack)
          if(!state->update(deltatime))
                break;

}

template<typename STATE_ID>
void StateManager<STATE_ID>::draw()
{
    for(auto it=m_stateStack.rbegin(); it!=m_stateStack.rend(); ++it)
    {
        ( *it )->render();
    }
}

template<typename STATE_ID>
void StateManager<STATE_ID>::pushState(STATE_ID stateId)
{
    mPendingList.push_back(PendingChange(Action::Push,stateId));
}

template<typename STATE_ID>
void StateManager<STATE_ID>::popState()
{
    mPendingList.push_back(PendingChange(Action::Pop));
}

template<typename STATE_ID>
void StateManager<STATE_ID>::clearState()
{
    mPendingList.push_back(PendingChange(Action::Clear));
}

template<typename STATE_ID>
bool StateManager<STATE_ID>::isEmpty()const
{
    return m_stateStack.empty();
}

template<typename STATE_ID>
ContextApp& StateManager<STATE_ID>::getContext()
{
    return m_context;
}


template<typename STATE_ID>
typename State<STATE_ID>::Ptr StateManager<STATE_ID>::createState(STATE_ID stateId)
{
    auto found = m_factories.find(stateId);
    assert(found != m_factories.end()&&"stateId no exist");
    return found->second();
}

template<typename STATE_ID>
void StateManager<STATE_ID>::applyChange()
{
    for (PendingChange change : mPendingList)
    {
        switch (change.m_action)
        {
            case Push:
            {
                if(m_stateStack.empty() or m_stateStack.front()->getStateId()!=change.m_stateID)
                        m_stateStack.push_front(createState(change.m_stateID));
            }
                break;

            case Pop:
            {
                if(!isEmpty())
                    m_stateStack.pop_front();
            }
                break;

            case Clear:
                m_stateStack.clear();
                break;
        }
    }

    mPendingList.clear();
}
