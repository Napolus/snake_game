//
// Created by napolus on 8/26/20.
//

#include "splashState.hpp"

SplashState::SplashState(StateManager<StateOfGame> &manager) :State(manager)
{
    initResources();
    initListener();
    initTexts();

    m_background.setTexture(manager.getContext().textures["SPLASHSCREEN"]);
    manager.getContext().window->setSize(m_background.getTexture()->getSize());


    auto monitor=sf::Vector2u(sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height);
    auto center=(monitor/2U)-(manager.getContext().window->getSize()/2U);
    manager.getContext().window->setPosition(sf::Vector2i(center));


    m_clock.restart();

}

void SplashState::initTexts()
{

}

void SplashState::initListener()
{


}
void SplashState::initResources()
{
    TexturesH &textures=m_state_manager.getContext().textures;
    textures.acquire("SPLASHSCREEN",thor::Resources::fromFile<sf::Texture>("../../Assets/images/splashscreen.jpg"));


    FontsH &fonts=m_state_manager.getContext().fonts;
    fonts.acquire("SCORE_FONT",thor::Resources::fromFile<sf::Font>("../../Assets/fonts/scoreFont.ttf"));
}

bool SplashState::processEvent()
{
    return false;
}

bool SplashState::update(const sf::Time deltatime)
{
    if(m_clock.getElapsedTime().asSeconds()>5.f)
    {
        requestPopState();
        requestPushState(StateOfGame::GAME);
    }
    return false;
}

bool SplashState::render()
{
    m_state_manager.getContext().window->draw(m_background);
    return false;
}

StateOfGame SplashState::getStateId() const
{
    return StateOfGame::SPLASH;
}

SplashState::~SplashState()
{
    m_state_manager.getContext().textures.release("SPLASHSCREEN");

    m_state_manager.getContext().fonts.release("SCORE_FONT");
}


