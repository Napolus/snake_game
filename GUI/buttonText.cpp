//
// Created by napolus on 8/24/20.
//

#include <SFML/Graphics/RenderTarget.hpp>
#include <utility>

#include "buttonText.hpp"

ButtonText::ButtonText(std::string  label,const sf::Font& font):InteractWidget(Type::Button),m_label(std::move(label))
{
    setColors(sf::Color::White,sf::Color::Cyan,sf::Color::Blue);
    m_onclick=[]{};
    m_onHover=[]{};
    m_onLeave=[]{};
    initText(font);
}

void ButtonText::initText(const sf::Font &font)
{
    m_text.setFont(font);
    m_text.setString(m_label);
    updateButtonText();
}

void ButtonText::setColors(const sf::Color &normal,const sf::Color &hover,const sf::Color &clicked)
{
    m_Normal_textColor=normal;
    m_Hover_textColor=hover;
    m_Clicked_textColor=clicked;
}

void ButtonText::setSize(const sf::Vector2f &size)
{
    //do nothing because can't set size of ButtonText ,you can use Button::setCharacterSize.
    //AbstractWidget::setSize(size);
}

void ButtonText::onMousePressed()
{
    m_text.setFillColor(m_Clicked_textColor);
}

void ButtonText::onMouseReleased()
{
    m_text.setFillColor(m_Hover_textColor);
    m_onclick();
}

void ButtonText::onMouseHover()
{
    m_text.setFillColor(m_Hover_textColor);
    m_onHover();
}

void ButtonText::onMouseLeave()
{
    m_text.setFillColor(m_Normal_textColor);
    m_onLeave();
}

void ButtonText::onGainFocus()
{

}

void ButtonText::onLostFocus()
{

}

bool ButtonText::contain(int x,int y) const
{
    const auto &rect=getTransform().transformRect(m_text.getGlobalBounds());
    return rect.contains(sf::Vector2f(x,y));
}

const sf::Vector2f &ButtonText::getSize() const
{
    return m_size;
}

void ButtonText::draw(sf::RenderTarget &target,sf::RenderStates states) const
{
    states.transform*=getTransform();
    target.draw(m_text,states);
}

const std::string &ButtonText::getLabel() const
{
    return m_label;
}

void ButtonText::setLabel(const std::string &label)
{
    m_label=label;
    m_text.setString(label);
    updateButtonText();
}

void ButtonText::onclick(const std::function<void()> &function)
{
    m_onclick=function;
}

void ButtonText::onHover(const std::function<void()> &function)
{
    m_onHover=function;
}

void ButtonText::onLeave(const std::function<void()> &function)
{
    m_onLeave=function;
}

void ButtonText::updateButtonText()
{
    const auto &rect=m_text.getGlobalBounds();
    m_size=sf::Vector2f(rect.width,rect.height);
}

void ButtonText::setCharacterSize(unsigned int size)
{
    m_text.setCharacterSize(size);
    updateButtonText();
}

void ButtonText::update(float dt)
{
    InteractWidget::update(dt);
}



