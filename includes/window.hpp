//
// Created by napolus on 7/10/20.
//

#ifndef SNAKE_GAME_WINDOW_HPP
#define SNAKE_GAME_WINDOW_HPP


#include <SFML/Graphics/RenderWindow.hpp>


class Window final : public sf::RenderWindow
{
public:
    Window();

    //create window with fixed size
    Window(const std::string& title,const sf::Vector2u& size);

    //create window with size relative to screen
    Window(const std::string& title,unsigned short widthPC,unsigned short heightPC);

    ~Window() override;

    void ProcessEvent();

    bool IsFullscreen() const;

    void setFullscreen(bool full);

    void ToggleFullscreen();

    void   setSize(const sf::Vector2u &new_size) ;

    void   setTitle(const sf::String &new_title) ;

private:
    void Setup(const std::string& title,const sf::Vector2u& size);
    void create(sf::VideoMode mode,const sf::String &title,sf::Uint32 style =sf::Style::Default,const sf::ContextSettings &  	settings = sf::ContextSettings());
    void create(sf::WindowHandle handle, const sf::ContextSettings & settings = sf::ContextSettings());
    void close();
    void Create();

private:
    sf::Vector2u m_windowSize;
    std::string m_windowTitle;
    bool m_isFullscreen{};
};

#endif //SNAKE_GAME_WINDOW_HPP
