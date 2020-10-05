//
// Created by napolus on 8/23/20.
//

#ifndef SNAKE_GAME_PAUSESTATE_HPP
#define SNAKE_GAME_PAUSESTATE_HPP

#include <SFML/Graphics/ConvexShape.hpp>
#include "ManageAction/ActionMap.hpp"
#include "enumerations.hpp"

#include "stateManager.hpp"
#include "state.hpp"
#include "../GUI/buttonText.hpp"

class PauseState :public State<StateOfGame>
{
public:

    explicit PauseState(StateManager<StateOfGame>& manager);
    ~PauseState() override;


    bool processEvent() override;

    bool update(const sf::Time deltatime) override;

    bool render() override;

    StateOfGame getStateId() const override {return StateOfGame::PAUSE;};

private:
    void initListener();

    void initResources();

private:
    enum ActionPauseGame
    {
        resume_game,
        quit
    };

    sf::ConvexShape                         m_rounded_background;

    thor::ActionMap<ActionPauseGame>         m_mapping_state;

    std::shared_ptr<ButtonText>             m_resumePlay;
    LamdaListener                           m_make_pause;

};


#endif //SNAKE_GAME_PAUSESTATE_HPP
