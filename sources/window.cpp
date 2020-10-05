//
// Created by napolus on 7/10/20.
//

#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "window.hpp"

using namespace std::string_literals;

Window::Window(): Window("Window"s,50,50)
{}

Window::Window(const std::string &title,const sf::Vector2u &size)
{
    Setup(title,size);
}

Window::Window(const std::string& title,unsigned short widthPC,unsigned short heightPC)
{

    widthPC=(widthPC>100)?sf::VideoMode::getDesktopMode().width: widthPC*sf::VideoMode::getDesktopMode().width/100;
    heightPC=(heightPC>100)?sf::VideoMode::getDesktopMode().height: heightPC*sf::VideoMode::getDesktopMode().height/100;

    Setup(title,{widthPC,heightPC});
}

Window::~Window()
{
    close();
}

void Window::ProcessEvent()
{
    sf::Event event{};
    while(this->pollEvent(event)){
        if(event.type == sf::Event::Closed){
            this->close();
        }
    }
}

bool Window::IsFullscreen() const
{
    return m_isFullscreen;
}


void Window::ToggleFullscreen()
{
    m_isFullscreen=!m_isFullscreen;
    Create();
}

void Window::Setup(const std::string &title,const sf::Vector2u &size)
{
    m_windowTitle = title;
    m_windowSize = size;
    m_isFullscreen = false;
    Create();
}

void Window::close()
{
    sf::RenderWindow::close();
}

void Window::Create()
{
    if(m_isFullscreen)
        this->create({ sf::VideoMode::getDesktopMode().width,sf::VideoMode::getDesktopMode().height},
                     m_windowTitle,
                     sf::Style::Fullscreen);
    else
    {
        this->create({m_windowSize.x,m_windowSize.y},m_windowTitle,sf::Style::None);
    }

    this->setVerticalSyncEnabled(true);
    this->setKeyRepeatEnabled(false);

}


void Window::setSize(const sf::Vector2u &new_size)
{
    if(m_windowSize!=new_size)
    {
        m_windowSize=new_size;
        Create();
    }
}

void Window::setFullscreen(bool full)
{
    if(full!=m_isFullscreen)
    {
        m_isFullscreen=full;
        Create();
    }
}

void Window::create(sf::VideoMode mode,const sf::String &title,sf::Uint32 style,const sf::ContextSettings &settings)
{
    sf::RenderWindow::create(mode,title,style,settings);
}

void Window::create(sf::WindowHandle handle,const sf::ContextSettings &settings)
{
    sf::RenderWindow::create(handle,settings);
}

void Window::setTitle(const sf::String &new_title)
{
    m_windowTitle=new_title;
    sf::RenderWindow::setTitle(new_title);
}

