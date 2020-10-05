//
// Created by napolus on 7/10/20.
//

#include <ManageAction/EventManager.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>



#include <thread>



#include "game.h"
#include "window.hpp"
#include "stateManager.hpp"
#include "state.hpp"
#include "pauseState.hpp"
#include "gameState.hpp"
#include "splashState.hpp"

using namespace std::string_literals;
using namespace std::chrono_literals;

Game::Game():m_contextApp(ContextApp()),
             m_stateManager(std::make_shared<StateManager<StateOfGame>>(m_contextApp)),
             main_mapping(nullptr),isRun(true)

{
    //make valid the contextApp
    m_contextApp.window=std::make_shared<Window>("snake game by napoleon",25,25);
    m_contextApp.event_manager=EventManager::Instance();
    main_mapping=thor::ActionMap<ActionGameEngine>(m_contextApp.event_manager);

    //register for listen event
    main_mapping[ActionGameEngine::onExit]=thor::Action(sf::Event::Closed)||thor::Action(sf::Keyboard::Escape,thor::Action::ReleaseOnce);
    main_mapping[ActionGameEngine::toggleFullScreen]=thor::Action(sf::Keyboard::F5,thor::Action::ReleaseOnce);

    m_make_exit.function=[=](const thor::ActionContext&){isRun=false;};
    m_make_exit.subscribe(ActionGameEngine::onExit);

    m_toggleFullScreen.function=[=](const thor::ActionContext&){m_contextApp.window->ToggleFullscreen();};
    m_toggleFullScreen.subscribe(ActionGameEngine::toggleFullScreen);


    //register all state of game
    m_stateManager->registerState<SplashState>(StateOfGame::SPLASH);
    m_stateManager->registerState<GameState>(StateOfGame::GAME);
    m_stateManager->registerState<PauseState>(StateOfGame::PAUSE);

    m_stateManager->pushState(StateOfGame::SPLASH);
}

Game::~Game(){ std::cout<<"le game est detruit"<<std::endl;};

void Game::ProcessEvents()
{
    m_contextApp.eventBuffer.pollEvents(m_contextApp.window);

    main_mapping.filtreEvents(m_contextApp.eventBuffer);
    m_stateManager->processEvent();
    m_contextApp.event_manager->DispatchEvents();

    m_contextApp.eventBuffer.clearEvents();
}


void Game::run(int minimum_frame_per_seconds)
{
    sf::Clock clockForRender;
    sf::Clock clockForUpdate;
    sf::Time timeSinceLastRender{sf::Time::Zero};
    const sf::Time TimePerFrame{sf::seconds(1.f/static_cast<float >(minimum_frame_per_seconds))};

    while (isRun)
    {
        ProcessEvents();
        while (timeSinceLastRender < TimePerFrame)
        {
            update(clockForUpdate.getElapsedTime());
            ProcessEvents();

            timeSinceLastRender += clockForUpdate.restart();
        }
        update(clockForUpdate.restart());

        render();
        timeSinceLastRender = clockForRender.restart();

    }

}

void Game::update(sf::Time deltatime)
{
        m_stateManager->update(deltatime);
}

void Game::render()
{
    std::this_thread::sleep_for(2500us);
    m_contextApp.window->clear(sf::Color::Red);
    m_stateManager->draw();
    m_contextApp.window->display();

}
