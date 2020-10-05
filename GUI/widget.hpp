//
// Created by napolus on 8/24/20.
//

#ifndef SNAKE_GAME_WIDGET_HPP
#define SNAKE_GAME_WIDGET_HPP


#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>

class AbstractWidget :public sf::Drawable ,public sf::Transformable
{
public:
    enum class Type
    {
        Button,
        Label
    };

    explicit AbstractWidget(Type type);;

    ~AbstractWidget() override = default;

    Type getType();

    virtual void setSize(const sf::Vector2f &size);

    bool isEnabled() const;

    void setEnabled(bool enabled);

    const std::string &getName() const;

    void setName(const std::string &name);

protected:
    virtual const sf::Vector2f& getSize()const=0;

protected:
    Type m_type;
    std::string m_name;

    sf::Vector2f m_size;

    bool m_enabled;

};


#endif //SNAKE_GAME_WIDGET_HPP
