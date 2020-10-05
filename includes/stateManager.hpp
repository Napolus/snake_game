//
// Created by napolus on 7/24/20.
//

#ifndef SNAKE_GAME_STATEMANAGER_HPP
#define SNAKE_GAME_STATEMANAGER_HPP

#include <SFML/System/NonCopyable.hpp>

#include <functional>
#include <deque>
#include <memory>

#include "enumerations.hpp"

template<typename STATE_ID>
class State;

template <typename STATE_ID>
class StateManager :sf::NonCopyable
{
public:
    enum Action { Push,Pop,Clear};

public:
    explicit StateManager(ContextApp& context):m_context(context){}

    ~StateManager()
    {
        m_factories.clear();
        m_stateStack.clear();
    }

    template <typename T,typename ...ARGS>
    void registerState(STATE_ID stateId,ARGS&& ...args);

    void processEvent();

    void update(sf::Time deltatime);

    void draw();

    void pushState(STATE_ID stateId);

    void popState();

    void clearState();

    [[nodiscard]] bool isEmpty()const;

    ContextApp& getContext();

private:
    typename  State<STATE_ID>::Ptr  createState(STATE_ID stateId);

    void applyChange();

private:
    struct PendingChange
    {
        PendingChange(Action action, STATE_ID stateId):m_action(action),m_stateID(stateId){}
        explicit PendingChange(Action action):m_action(action),m_stateID(){}
        Action      m_action;
        STATE_ID    m_stateID;
    };

private:
    using Container = std::deque<typename State<STATE_ID>::Ptr>;
    using StateFactorie=std::unordered_map<STATE_ID,std::function<typename State<STATE_ID>::Ptr()>>;

private:
    Container                               m_stateStack;
    StateFactorie                           m_factories;
    std::vector<PendingChange>              mPendingList;
    ContextApp&                             m_context;
};




#include "stateManager.inl"

#endif //SNAKE_GAME_STATEMANAGER_HPP
