//
// Created by napolus on 7/18/20.
//

#ifndef SNAKE_GAME_EVENTMANAGER_HPP
#define SNAKE_GAME_EVENTMANAGER_HPP

#include <map>
#include <queue>
#include <stack>
#include <cassert>
#include <SFML/System/Clock.hpp>
#include <iostream>

#include "ActionContext.hpp"
#include "eventID.hpp"

class Listener;


class EventManager{
public:
    using Map=std::multimap<ID::EVENT_ID,Listener& >;
    using Queue= std::queue<thor::ActionContext>;

    EventManager(const EventManager& other)= delete;

    EventManager operator=(const EventManager& other)= delete;

    ~EventManager()= default;

private:
    EventManager()= default;

public:
    using EventManagerPtr=std::shared_ptr<EventManager>;

    static EventManagerPtr Instance();

    template<typename T>
    void Register(Listener& listener,T type);

    void Unregister(Listener& listener,ID::EVENT_ID eventId);

    void Unregister(Listener& listener);

    void Dispatch(thor::ActionContext& event);

    void DispatchEvents();

    void SendEvent(const thor::ActionContext& event);

private:
    template<typename T>
    bool AlreadyRegisted(Listener& listener,T type);

private:
    Queue m_events;
    Map m_listenerMap;

};

class Listener{
public:
    Listener()= default;

     virtual ~Listener()
     {
         unsubscribe();
     }

    template<typename  EVENT_TYPE>
    void subscribe(EVENT_TYPE type)
    {
        EventManager::Instance()->Register(*this,type);
    }

    template<typename  EVENT_TYPE>
    void unsubscribe(EVENT_TYPE type)
    {
        EventManager::Instance()->Unregister(*this,type);
    }

    void unsubscribe()
    {
        EventManager::Instance()->Unregister(*this);
    }

    virtual void onHandleEvent(thor::ActionContext  event) {m_stackEventsFromSubscribe.push(std::move(event));};

protected:
    std::stack<thor::ActionContext> m_stackEventsFromSubscribe;
};


class LamdaListener : public Listener
{

public:
    LamdaListener()= default;
    explicit LamdaListener(std::function<void(thor::ActionContext)> functionToExecute):function(std::move(functionToExecute)){}
    ~LamdaListener()  = default;
    std::function<void(thor::ActionContext)> function;

    void onHandleEvent(thor::ActionContext event) override
    {
        function(std::move(event));
    }
};



inline EventManager::EventManagerPtr EventManager::Instance()
{
    static auto Instance=std::shared_ptr<EventManager>(new EventManager());
    return Instance;
}

template< typename T>
inline void EventManager::Register(Listener& listener,T type)
{
    if(!AlreadyRegisted(listener,type))
        m_listenerMap.emplace(ID::getEventId(type),listener);
}

inline void EventManager::Unregister(Listener& listener,ID::EVENT_ID eventId)
{
    auto range= m_listenerMap.equal_range(eventId);
    for(auto beg=range.first;beg!=range.second;++beg)
    {
        if( &(beg->second) == &listener)
        {
            m_listenerMap.erase(beg);
            return;
        }
    }
}

inline void EventManager::Unregister(Listener& listener)
{
   //todo std::remove_if(m_listenerMap.begin(),m_listenerMap.end(),[&listener](auto &iter){return iter
   // .second==listener;});
   auto iter=m_listenerMap.begin();
    while(iter!=m_listenerMap.end())
    {
        if( &(iter->second) == &listener )
            iter=m_listenerMap.erase(iter);
        else
            ++iter;
    }
}

inline void EventManager::Dispatch(thor::ActionContext& event)
{
    auto range= m_listenerMap.equal_range(event.actionId);
    for(auto beg=range.first;beg!=range.second;++beg)
    {
        //todo std::cout<<"*********************************"<<beg->second.use_count()<<std::endl;
        beg->second.onHandleEvent(event);
    }
}

inline void EventManager::DispatchEvents()
{
    while(!m_events.empty())
    {
        Dispatch(m_events.front());
        m_events.pop();
    }

}

template<typename T>
inline bool EventManager::AlreadyRegisted(Listener& listener,T type)
{
    auto range= m_listenerMap.equal_range(ID::getEventId(type));
    auto exist=std::any_of(range.first,range.second,[&listener](const auto &it){return &it.second==&listener;});

    return exist;
}

inline void EventManager::SendEvent(const thor::ActionContext& event)
{
    m_events.push(event);
}


#endif //SNAKE_GAME_EVENTMANAGER_HPP
