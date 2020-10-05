//
// Created by napolus on 8/23/20.
//

#include <costumColors.hpp>
#include <tools.h>
#include "pauseState.hpp"
#include "Shapes.hpp"

PauseState::PauseState(StateManager<StateOfGame>& manager) :State(manager),m_mapping_state(manager.getContext().event_manager)
{
    initResources();
    m_rounded_background=thor::Shapes::roundedRect(sf::Vector2f(manager.getContext().window->getSize()),
                                                   50.f,
                                                   50);
    m_rounded_background.setFillColor(sf::Color(0,0,0,150));

    m_resumePlay=std::make_shared<ButtonText>("press space to continue",manager.getContext().fonts["SCORE_FONT"]);
    m_resumePlay->setOrigin(m_resumePlay->getSize()/2.f);
    m_resumePlay->setPosition( sf::Vector2f(manager.getContext().window->getSize())/2.f);

    initListener();
}

PauseState::~PauseState()
{
        std::cout<<"destroy pauseState "<<std::endl;
}

bool PauseState::processEvent()
{
    m_mapping_state.filtreEvents(m_state_manager.getContext().eventBuffer);
    for(const auto & event:m_state_manager.getContext().eventBuffer.getBuffer())
    {
        if(!m_resumePlay->handleEvent(event))
            break;
    }
    return false;
}

bool PauseState::update(const sf::Time deltatime)
{
    m_resumePlay->update(0.f);
    return false;
}

bool PauseState::render()
{
    m_state_manager.getContext().window->draw(m_rounded_background);
    m_state_manager.getContext().window->draw(*m_resumePlay);
    return true;
}

void PauseState::initListener()
{
    m_mapping_state[ActionPauseGame::resume_game]=thor::Action(sf::Keyboard::Space,thor::Action::PressOnce);

    m_make_pause.function=[=](const thor::ActionContext&){requestPopState();};
    m_make_pause.subscribe(ActionPauseGame::resume_game);

    m_resumePlay->onclick([=]{
                                if (m_state_manager.getContext().cursor.loadFromSystem(sf::Cursor::Arrow))
                                    this->m_state_manager.getContext().window->setMouseCursor(m_state_manager.getContext().cursor);
                                requestPopState();
                             });
    m_resumePlay->onHover([=]{
                                if (m_state_manager.getContext().cursor.loadFromSystem(sf::Cursor::Hand))
                                        this->m_state_manager.getContext().window->setMouseCursor(m_state_manager.getContext().cursor);
                             });
    m_resumePlay->onLeave([=]{
                                if (m_state_manager.getContext().cursor.loadFromSystem(sf::Cursor::Arrow))
                                        this->m_state_manager.getContext().window->setMouseCursor(m_state_manager.getContext().cursor);
                             });
}

void PauseState::initResources()
{
    FontsH &fonts=m_state_manager.getContext().fonts;
    fonts.acquire("SCORE_FONT",
                  thor::Resources::fromFile<sf::Font>("../../Assets/fonts/scoreFont.ttf"),
                  thor::Resources::KnownIdStrategy::Reuse);
}
