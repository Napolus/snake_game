//
// Created by napolus on 7/24/20.
//

#include <iostream>

#include <SFML/Graphics/Texture.hpp>

#include "costumColors.hpp"

#include "gameState.hpp"
#include "window.hpp"

#include "world.h"
#include "snake.h"

using namespace std::placeholders;

GameState::GameState(StateManager<StateOfGame>& manager):State(manager),
                                                                          m_mapping_snake(manager.getContext().event_manager),
                                                                          m_mapping_state(manager.getContext().event_manager),
                                                                          m_snake_speed(0),m_live(3),m_score(0),
                                                                          m_has_lost(false),m_is_start(false)
{
    manager.getContext().window->setSize(sf::Vector2u(1000,700));

    initResources();

    m_world=std::make_shared<World>(*this,manager.getContext(),(sf::Vector2f)manager.getContext().window->getSize());

    initTexts();

    initListener();

    auto monitor=sf::Vector2u(sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height);
    auto center=(monitor/2U)-(manager.getContext().window->getSize()/2U);
    manager.getContext().window->setPosition(sf::Vector2i(center));
}

void GameState::initTexts()
{
    m_score_text=sf::Text("",m_state_manager.getContext().fonts["SCORE_FONT"],15);
    m_score_text.setPosition(16.f,16.f);
    m_score_text.setFillColor(color::Gold);

    m_live_text=sf::Text("",m_state_manager.getContext().fonts["SCORE_FONT"],15);
    m_live_text.setPosition(16.f,30.f);

    m_speed_text=sf::Text("",m_state_manager.getContext().fonts["SCORE_FONT"],15);
    m_speed_text.setPosition(16.f,45.f);
    m_speed_text.setFillColor(color::LightBlue);

    m_rectTexts.setSize(sf::Vector2f(60.f,50.f));
    m_rectTexts.setPosition(16.f,16.f);
    m_rectTexts.setFillColor(color::SkyBlue);
}


void GameState::initResources()
{
    TexturesH &textures=m_state_manager.getContext().textures;
    textures.acquire("SNAKE",thor::Resources::fromFile<sf::Texture>("../../Assets/images/snake_sprite.png"));
    textures.acquire("BACKGROUND",thor::Resources::fromFile<sf::Texture>("../../Assets/images/background.png"));
    textures.acquire("APPLE",thor::Resources::fromFile<sf::Texture>("../../Assets/images/apple.png"));


    FontsH &fonts=m_state_manager.getContext().fonts;
    fonts.acquire("SCORE_FONT",thor::Resources::fromFile<sf::Font>("../../Assets/fonts/scoreFont.ttf"));
}

void GameState::initListener()
{
    auto managerEvent=m_state_manager.getContext().event_manager;

    m_mapping_snake[SnakeDirection::Right]=thor::Action(sf::Keyboard::Right) || thor::Action(sf::Keyboard::D);
    m_mapping_snake[SnakeDirection::Left]=thor::Action(sf::Keyboard::Left) || thor::Action(sf::Keyboard::A);
    m_mapping_snake[SnakeDirection::Up]=thor::Action(sf::Keyboard::Up) || thor::Action(sf::Keyboard::W);
    m_mapping_snake[SnakeDirection::Down]=thor::Action(sf::Keyboard::Down) || thor::Action(sf::Keyboard::S);

    m_mapping_state[ActionPlayGame::has_lost]=thor::realtimeAction(std::bind(&World::playerCrash,m_world.get()));
    m_mapping_state[ActionPlayGame::make_pause]=thor::Action(sf::Keyboard::Space,thor::Action::PressOnce);

    m_make_pause.function=std::bind(&GameState::makePause,this,_1);
    m_make_pause.subscribe(ActionPlayGame::make_pause);

    m_make_lost.function=[=](const thor::ActionContext&){m_has_lost=true;};
    m_make_lost.subscribe(ActionPlayGame::has_lost);


}

bool GameState::processEvent()
{
    m_mapping_snake.filtreEvents(m_state_manager.getContext().eventBuffer);
    m_mapping_state.filtreEvents(m_state_manager.getContext().eventBuffer);

    return true;
}

bool GameState::update(const sf::Time deltatime)
{

    if(m_has_lost)
        return true;

    m_world->update(deltatime);
    m_score_text.setString("score :"+std::to_string(m_score));
    m_live_text.setString("lives : "+std::to_string(m_live));
    m_speed_text.setString("speed : "+std::to_string(static_cast<int>(m_world->m_player->getSpeed())));

    return true;
}

bool GameState::render()
{

    m_state_manager.getContext().window->draw(*m_world);
    m_state_manager.getContext().window->draw(m_rectTexts);
    m_state_manager.getContext().window->draw(m_score_text);
    m_state_manager.getContext().window->draw(m_live_text);
    m_state_manager.getContext().window->draw(m_speed_text);

    /*sf::CircleShape c(.5f);
    c.setFillColor(sf::Color::Red);
    for(int i=0; i<m_state_manager->getContext().window->getWindowSize().x; i+=16)
        for(int j=0; j<m_state_manager->getContext().window->getSize().y; j+=16)
        {
            c.setPosition(i,j);
            m_state_manager->getContext().window->draw(c);
        }*/

    return true;
}


void GameState::UpdateScore(int score)
{
    m_score+=score;
    if(m_score<0)
        m_score=0;
}

void GameState::UpdateLive(int live)
{
    m_live+=live;
    if(m_live<0)
        m_live=0;
}

GameState::~GameState()
{
    m_state_manager.getContext().textures.release("SNAKE");
    m_state_manager.getContext().textures.release("BACKGROUND");
    m_state_manager.getContext().textures.release("APPLE");

    m_state_manager.getContext().fonts.release("SCORE_FONT");
    std::cout<<"le gameState est detruit"<<std::endl;
}

void GameState::makePause(thor::ActionContext)
{
    requestPushState(StateOfGame::PAUSE);
}




