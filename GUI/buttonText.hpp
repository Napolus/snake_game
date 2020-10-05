//
// Created by napolus on 8/24/20.
//

#ifndef SNAKE_GAME_BUTTONTEXT_HPP
#define SNAKE_GAME_BUTTONTEXT_HPP

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "interactWidget.hpp"

class ButtonText : public InteractWidget
{

public:

    ButtonText(std::string  label,const sf::Font& font);

    ButtonText()= delete;

    ~ButtonText() override = default;

    void setColors(const sf::Color &normal,const sf::Color &hover,const sf::Color &clicked);

    void setCharacterSize (unsigned int size);

    const sf::Vector2f &getSize() const override;

    void update(float dt) override ;

    //callbacks
    void onclick(const std::function<void()> &function);
    void onHover(const std::function<void()> &function);
    void onLeave(const std::function<void()> &function);

private:
    void initText(const sf::Font &font);
    void updateButtonText();

protected:
    void onGainFocus() override;

    void onLostFocus() override;

    void onMousePressed() override;

    void onMouseReleased() override;

    void onMouseHover() override;

    void onMouseLeave() override;

    bool contain(int x,int y) const override;

    void draw(sf::RenderTarget &target,sf::RenderStates states) const override;

    const std::string &getLabel() const;

    void setLabel(const std::string &label);

    void setSize(const sf::Vector2f &size) override;

private:
    std::string     m_label;
    sf::Text        m_text;

    sf::Color  m_Normal_textColor;
    sf::Color  m_Hover_textColor;
    sf::Color  m_Clicked_textColor;

    std::function<void()>    m_onclick;
    std::function<void()>    m_onHover;
    std::function<void()>    m_onLeave;
};


#endif //SNAKE_GAME_BUTTONTEXT_HPP
