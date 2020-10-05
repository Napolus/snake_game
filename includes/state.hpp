//
// Created by napolus on 7/24/20.
//

#ifndef SNAKE_GAME_STATE_HPP
#define SNAKE_GAME_STATE_HPP

#include <SFML/System/Time.hpp>
#include <memory>

template <typename STATE_ID>
class StateManager;



template <typename STATE_ID>
class State
{
public:
    using Ptr=std::unique_ptr<State>;

    explicit State(StateManager<STATE_ID>& state_manager):m_state_manager(state_manager){}
    virtual ~State()= default;

    virtual bool processEvent()=0;

    virtual bool update(const sf::Time deltatime)=0;

    virtual bool render()=0;

    virtual STATE_ID getStateId() const =0;

protected:
    void requestPushState(STATE_ID stateId);

    void requestPopState();

    void requestClearState();


protected:
    StateManager<STATE_ID>& m_state_manager;
};


#include "state.inl"

#endif //SNAKE_GAME_STATE_HPP
