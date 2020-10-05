//
// Created by napolus on 7/12/20.
//

#ifndef SNAKE_GAME_WORLD_H
#define SNAKE_GAME_WORLD_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>


#include "enumerations.hpp"

class Snake;

namespace sf
{
    class RenderTarget;
}

class GameState;

class World :public sf::Drawable
{
    friend class GameState;
public:
    explicit World(GameState& gameState,ContextApp &context,sf::Vector2f size,unsigned blockSize=16);

    ~World() override;

    float getBlockSize() const;

    void spawnApple();

    void update(sf::Time deltatime);

    const sf::Vector2f& getSize() const;

    bool playerCrash();

private:
    bool eatBonus() const ;
    bool playerCollideBonds(std::shared_ptr<Snake> player);

protected:
    void draw(sf::RenderTarget &target,sf::RenderStates states) const override;

private:
    sf::Vector2f                m_size;
    float                       m_blockSize;

    sf::Sprite                  m_apple;
    sf::Vector2f                m_posApple;

    std::shared_ptr<Snake>      m_player;
    bool                        m_playerCrash;

    sf::Sprite                  m_background;
    sf::RectangleShape          m_bounds[4];

    ContextApp&                 m_context;
    GameState&                  m_gameState;

};


#endif //SNAKE_GAME_WORLD_H
