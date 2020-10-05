//
// Created by napolus on 8/26/20.
//

#ifndef SNAKE_GAME_SPLASHSTATE_HPP
#define SNAKE_GAME_SPLASHSTATE_HPP


#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "enumerations.hpp"
#include "state.hpp"

class SplashState :public State<StateOfGame>
{
public:
    explicit SplashState(StateManager<StateOfGame> &manager);

    ~SplashState() override;

    bool processEvent() override;

    bool update(const sf::Time deltatime) override;

    bool render() override;

    StateOfGame getStateId() const override;

private:
    void initTexts();

    void initListener();

    void initResources();

private:
    sf::Sprite m_background;
    sf::Clock m_clock;

};


#endif //SNAKE_GAME_SPLASHSTATE_HPP
