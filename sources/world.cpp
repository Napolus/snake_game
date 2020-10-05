//
// Created by napolus on 7/12/20.
//

#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>
#include <cmath>

#include "costumColors.hpp"
#include "gameState.hpp"
#include "world.h"
#include "tools.h"
#include "snake.h"

World::World(GameState& gameState,ContextApp& context,sf::Vector2f size,unsigned blockSize):m_size(size),
                                                                                                  m_blockSize(blockSize),m_playerCrash(false),
                                                                                                  m_context(context),m_gameState(gameState)
{
    m_player=std::make_shared<Snake>(m_blockSize,m_context.textures["SNAKE"]);
    m_player->reset(sf::Vector2f(2,2),SnakeDirection::Down);
    m_player->setSpeed(0.f);
    m_player->subscribe(SnakeDirection::Up);
    m_player->subscribe(SnakeDirection::Down);
    m_player->subscribe(SnakeDirection::Right);
    m_player->subscribe(SnakeDirection::Left);

    m_apple.setTexture(m_context.textures["APPLE"]);
    sf::Vector2f scaleA=static_cast<sf::Vector2f>(m_apple.getTexture()->getSize())/(1.f*m_blockSize);
    m_apple.scale(1.f/scaleA.x,1.f/scaleA.y);
    m_apple.setOrigin(sf::Vector2f(m_apple.getTexture()->getSize())/2.f);
    spawnApple();

    m_background.setTexture(m_context.textures["BACKGROUND"]);
    sf::Vector2f scaleB;
    scaleB.x=(m_background.getTexture()->getSize().x)/( (float)m_size.x );
    scaleB.y=(m_background.getTexture()->getSize().y)/( (float)m_size.y );
    m_background.scale(1.f/scaleB.x,1.f/scaleB.y);


    m_bounds[0].setPosition(0,0);//on top
    m_bounds[0].setSize({m_size.x,m_blockSize});

    m_bounds[1].setPosition(0,0);//on left
    m_bounds[1].setSize({m_blockSize,m_size.y});

    double integerx;
    double integery;
    std::modf(m_size.x/m_blockSize,&integerx);
    std::modf(m_size.y/m_blockSize,&integery);

    if((m_size.x-integerx*m_blockSize)<m_blockSize/2.f)
        --integerx;

    if((m_size.y-integery*m_blockSize)<m_blockSize/2.f)
        --integery;

    m_bounds[2].setPosition(integerx*m_blockSize,0);//on right
    m_bounds[2].setSize({static_cast<float>(m_size.x-integerx*m_blockSize),m_size.y});

    m_bounds[3].setPosition(0,integery*m_blockSize);//on bottom
    m_bounds[3].setSize({m_size.x,static_cast<float>(m_size.y-integery*m_blockSize)});

    for(auto &bound:m_bounds)
    {
        bound.setFillColor(color::RoyalBlue);
    }
}

World::~World(){std::cout<<"le world est detruit"<<std::endl;};

float World::getBlockSize() const
{
    return m_blockSize;
}

void World::spawnApple()
{
    do
    {
        int xPos=tools::Randomize(1U,( static_cast<unsigned>(m_size.x/m_blockSize)) - 2);
        int yPos=tools::Randomize(1U,( static_cast<unsigned>(m_size.y/m_blockSize)) - 2);
        m_posApple=sf::Vector2f (xPos,yPos);
    }
    while(std::any_of(m_player->m_snakeBody.cbegin(),m_player->m_snakeBody.cend(),
                      [this](auto segment){return segment.m_position==m_posApple; }));


    m_posApple*=m_blockSize;
    m_posApple+=sf::Vector2f(m_blockSize/2.f,m_blockSize/2.f);
    m_apple.setPosition(m_posApple);
}

void World::draw(sf::RenderTarget &target,sf::RenderStates states) const
{
    target.draw(m_background);
    for(auto &bound:m_bounds)
        target.draw(bound);
    target.draw(m_apple);
    m_player->render((Window &)target);

    /*sf::CircleShape c(1);
    c.setFillColor(sf::Color::Blue);
    c.setPosition(m_apple.getPosition());
    target.draw(c);*/
};

const sf::Vector2f& World::getSize() const
{
    return m_size;
}

void World::update(sf::Time deltatime)
{
    if(m_gameState.m_mapping_snake.isActive(m_player->getDirection(),m_gameState.m_state_manager.getContext().eventBuffer))
    {
        if(!m_gameState.m_is_start)
            m_gameState.m_is_start=true;
        m_player->updateSpeed(m_player->getSpeed() + 10*deltatime.asSeconds());
    }
    else if(m_gameState.m_is_start)
    {
        m_player->updateSpeed(m_player->getSpeed() - 15*deltatime.asSeconds());
    }

    auto backupBody=m_player->m_snakeBody;
    m_player->update(deltatime);

    if(playerCollideBonds(m_player))
    {
        m_player->m_snakeBody=backupBody;
        m_playerCrash=true;
        return;
    }

    if(eatBonus())
    {
        m_player->grow();
        m_gameState.UpdateScore(1);
        spawnApple();
    }
}

bool World::playerCollideBonds(std::shared_ptr<Snake> player)
{
    for(const auto &bond:m_bounds)
    {
        if(bond.getGlobalBounds().contains(player->getPosition()))
        {
            return true;
        }
    }
    return false;
}

bool World::playerCrash()
{
    if(m_player->m_can_cut and m_gameState.m_live>0 and m_player->isCollide())
    {
        m_gameState.UpdateLive(-1);
        return m_playerCrash;
    }
    return m_playerCrash or m_player->isCollide();
}

bool World::eatBonus() const
{
    return m_player->getPosition()==m_posApple;
}
