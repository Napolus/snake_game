//
// Created by napolus on 8/24/20.
//

#ifndef SNAKE_GAME_INTERACTWIDGET_HPP
#define SNAKE_GAME_INTERACTWIDGET_HPP

#include "widget.hpp"

class InteractWidget : public AbstractWidget
{
public:
    enum class State
    {
        None,
        Hover,
        Clicked,
        Released,
        Leave
    };

    explicit InteractWidget(Type type);

    ~InteractWidget() override = default;

    bool isVisible() const;

    void setVisible(bool visible);

    virtual bool canFocused() const;

    bool isFocused() const;

    void setFocused(bool focused);

    virtual bool handleEvent(const sf::Event& event);

    virtual void update(float dt);

protected:

    virtual void onMousePressed()=0;
    virtual void onMouseReleased()=0;
    virtual void onMouseHover()=0;
    virtual void onMouseLeave()=0;
    virtual void onGainFocus()=0;
    virtual void onLostFocus()=0;

    virtual bool contain(int x,int y) const= 0;

private:
    State m_current_state;
    State m_next_state;

    bool m_focused;
    bool m_gain_focus;
    bool m_lost_focus;

    bool m_visible;

};


#endif //SNAKE_GAME_INTERACTWIDGET_HPP
