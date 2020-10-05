//
// Created by napolus on 7/24/20.
//

#ifndef SNAKE_GAME_GAMESTATE_HPP
#define SNAKE_GAME_GAMESTATE_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>


#include "ManageAction/ActionMap.hpp"
#include "enumerations.hpp"

#include "stateManager.hpp"
#include "state.hpp"


class World;
class Snake;

class GameState :public State<StateOfGame>
{
    friend class World;
public:

    explicit GameState(StateManager<StateOfGame>& manager);
    ~GameState() override;


    bool processEvent() override;

    bool update(const sf::Time deltatime) override;

    bool render() override;

    StateOfGame getStateId() const override {return StateOfGame ::GAME;};

private:

    enum ActionPlayGame
    {
        start,
        eat_bonus,
        make_pause,
        has_lost,
    };
    void UpdateScore(int score);

    void UpdateLive(int live);

    void initTexts();

    void initListener();

    void initResources();

    void makePause(thor::ActionContext);

private:
    std::shared_ptr<World>                          m_world;
    thor::ActionMap<SnakeDirection>                 m_mapping_snake;
    thor::ActionMap<ActionPlayGame>                 m_mapping_state;


    sf::RectangleShape  m_rectTexts;
    sf::Text  m_score_text;
    sf::Text  m_live_text;
    sf::Text  m_speed_text;

    float     m_snake_speed;
    unsigned m_live;
    int m_score;
    bool m_has_lost;

    bool m_is_start;

    LamdaListener m_make_pause;
    LamdaListener m_make_lost;
};


#endif //SNAKE_GAME_GAMESTATE_HPP
