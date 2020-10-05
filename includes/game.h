//
// Created by napolus on 7/10/20.
//

#ifndef SNAKE_GAME_GAME_H
#define SNAKE_GAME_GAME_H


#include "enumerations.hpp"


template<typename STATE_ID>
class StateManager;


class Game
{
public:
    Game();

    virtual ~Game();

    /**
     * @brief launch game loop
     */
    void run(int minimum_frame_per_seconds);

private:
    void ProcessEvents();

    void update(sf::Time deltatime);

    void render();

private:
    ContextApp                                      m_contextApp;
    std::shared_ptr<StateManager<StateOfGame>>      m_stateManager;

    thor::ActionMap<ActionGameEngine>               main_mapping;

    LamdaListener                                   m_make_exit;
    LamdaListener                                   m_toggleFullScreen;

    bool isRun;
};


#endif //SNAKE_GAME_GAME_H
