//
// Created by napolus on 7/24/20.
//

#ifndef SNAKE_GAME_ENUMERATIONS_HPP
#define SNAKE_GAME_ENUMERATIONS_HPP

#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include "Resources.hpp"
#include "ManageAction/EventManager.hpp"
#include "ManageAction/ActionMap.hpp"

/**************************************ENUMERATIONS**************************************************/

enum class StateOfGame
{
    SPLASH,MENU,GAME,PAUSE,GAME_OVER
};

enum class SnakeDirection
{
    Up,Down,Left,Right,None
};

enum class SnakeBodyPart
{
    Head,
    tree,
    tree_RightToUp,
    tree_DownToLeft,
    tree_UpToLeft,
    tree_RightToDown,
    tree_LeftToDown,
    tree_UPToRight,
    tree_DownToRight,
    tree_LeftToUP,
    Tail
};

enum class ActionGameEngine
{
    onExit,
    toggleFullScreen,
};

/**************************************ALIAS TYPES**************************************************/

namespace sf
{
    class Window;

    class Texture;
}
class Window;

using WindowPtr=thor::ActionContext::WindowPtr ;
using TexturesH=thor::ResourceHolder<sf::Texture,std::string>;
using FontsH=thor::ResourceHolder<sf::Font,std::string>;


struct ContextApp
{
    ContextApp():window(nullptr),
                 event_manager(nullptr){}

     //cursor for window should destroyed after window for avoid undefined behavior
    sf::Cursor                              cursor;

    WindowPtr                               window;
    TexturesH                               textures;
    FontsH                                  fonts;
    EventManager::EventManagerPtr           event_manager;
    thor::detail::EventBuffer               eventBuffer;
};

#endif //SNAKE_GAME_ENUMERATIONS_HPP
