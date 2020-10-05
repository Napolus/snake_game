//
// Created by napolus on 8/24/20.
//

#include "interactWidget.hpp"

InteractWidget::InteractWidget(AbstractWidget::Type type) :AbstractWidget(type),m_current_state(State::None),
                                                            m_next_state(State::None),m_focused(true),m_visible(true),
                                                            m_gain_focus(false),m_lost_focus(false)
{

}

bool InteractWidget::isVisible() const
{
    return m_visible;
}

void InteractWidget::setVisible(bool visible)
{
    m_visible=visible;
}

bool InteractWidget::canFocused() const
{
    return true;
}


bool InteractWidget::isFocused() const
{
    return m_focused;
}

void InteractWidget::setFocused(bool focused)
{
    if(! canFocused())
        return;

    if(focused and !m_focused)
    {
        m_gain_focus=true;
        m_focused=focused;
    }
    else if(!focused and m_focused)
    {
        m_lost_focus=true;
        m_focused=focused;
    }

}

bool InteractWidget::handleEvent(const sf::Event &event)
{
    if(!isEnabled() or !isVisible())
        return false;

    if(event.type==sf::Event::MouseMoved)
    {
        if(contain(event.mouseMove.x,event.mouseMove.y))
        {
            m_next_state=State::Hover;
            return true;
        }
        else
        {
            if(m_current_state==State::Hover)
            {
                m_next_state=State::Leave;
                return true;
            }
            return false;
        }
    }
    if(m_current_state==State::Hover)
    {
        if(event.type==sf::Event::MouseButtonPressed)
        {
            m_next_state=State::Clicked;
            return true;
        }
        if(event.type==sf::Event::MouseButtonReleased)
        {
            m_next_state=State::Released;
            return true;
        }
    }
    return false;
}

void InteractWidget::update(float dt)
{
    if(m_gain_focus)
    {
        onGainFocus();
        m_gain_focus=false;
        return;
    }

    if(m_lost_focus)
    {
        onLostFocus();
        m_lost_focus=false;
        return;
    }

   if(m_next_state==State::Clicked)
   {
       m_next_state=State::None;
       onMousePressed();
       return;
   }

   if(m_next_state==State::Released)
    {
        m_next_state=State::None;
        onMouseReleased();
        return;
    }

   if(m_next_state==State::Hover and m_current_state!=State::Hover)
   {
       m_current_state=m_next_state;
       m_next_state=State ::None;
       onMouseHover();
       return;
   }

   if(m_next_state==State::Leave and m_current_state==State::Hover)
   {
       m_current_state=m_next_state=State ::None;
       onMouseLeave();
       return;
   }

}
