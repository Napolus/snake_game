//
// Created by napolus on 7/10/20.
//

#include <iostream>
#include <chrono>
#include <window.hpp>

#include <SFML/Graphics/CircleShape.hpp>
#include <cmath>

#include "snake.h"
#include "tools.h"

#include "ManageAction/eventID.hpp"

Snake::Snake(float sizeByBlock,sf::Texture& texture,const sf::Vector2f& startPoint) :m_texture(texture),m_sizeBlock(sizeByBlock),
                                                                                     m_dtAnimationHead(0.f),m_currentDirection(SnakeDirection::Right),
                                                                                     m_previewDirection(SnakeDirection::None),m_timeToUpdate(0.f),
                                                                                     m_speed(0.f),m_can_cut(false),
                                                                                     m_collide(false)


{
    initSprite();
    reset(startPoint,m_currentDirection);
}

void Snake::initSprite()
{
    m_bodySprite.setTexture(m_texture);

    m_texture.setSmooth(true);
    sf::Vector2f scale;
    scale.x=( 3.f*m_sizeBlock)/(m_texture.getSize().x);
    scale.y=( 3.f*m_sizeBlock)/(m_texture.getSize().y);

    m_bodySprite.setScale(scale);
    m_bodySprite.setOrigin(sf::Vector2f(m_texture.getSize()/3U)/2.f);
    m_bodySprite.setColor(sf::Color::Red);
}

void Snake::update(sf::Time deltatime)
{
    m_dtAnimationHead+=deltatime.asSeconds();
    if(m_dtAnimationHead>=( 1.f/4.f))
    {
        m_currentHeadFrame.top+=m_texture.getSize().y/3.f;
        if(m_currentHeadFrame.top==m_texture.getSize().y)
            m_currentHeadFrame.top=0;
        m_dtAnimationHead-=( 1.f/4.f);
    }

    m_timeToUpdate+=deltatime.asSeconds();
    if(m_timeToUpdate>1.f/m_speed)
    {
        Move();
        m_collide=checkCollision();
        m_timeToUpdate=0;
    }
}

void Snake::grow()
{
    if(m_snakeBody.empty()or m_currentDirection==SnakeDirection::None)
            return;

    SnakeSegment &tail_head=m_snakeBody.back();
    SnakeSegment &tail_bone=m_snakeBody.at(m_snakeBody.size() - 2);

    float rotation;
    sf::Vector2f position;

    switch(detectDirection(tail_bone,tail_head))
    {

        case SnakeDirection::Up:
        {
            rotation=0.f;
            position={tail_head.m_position.x,tail_head.m_position.y + 1};
        }
            break;
        case SnakeDirection::Down:
        {
            rotation=0.f;
            position={tail_head.m_position.x,tail_head.m_position.y - 1};
        }
            break;
        case SnakeDirection::Left:
        {
            rotation=90.f;
            position={tail_head.m_position.x + 1,tail_head.m_position.y};
        }
            break;
        case SnakeDirection::Right:
        {
            rotation=90.f;
            position={tail_head.m_position.x - 1,tail_head.m_position.y};
        }
            break;
        case SnakeDirection::None:
            return;
    }

    m_snakeBody.back().m_body_part=SnakeBodyPart::tree;
    m_snakeBody.back().m_rotation=rotation;
    m_snakeBody.push_back(SnakeSegment(position.x,position.y,SnakeBodyPart::Tail));
}

bool Snake::checkCollision()
{
    if(m_snakeBody.size()<5)
        return false;

    for(auto it=m_snakeBody.begin() + 4; it!=m_snakeBody.end(); ++it)
    {
        if(m_snakeBody.front().m_position==it->m_position)
        {
            if(m_can_cut)
            {
                m_snakeBody.erase(it,m_snakeBody.end());
                m_snakeBody.back().m_body_part=SnakeBodyPart::Tail;
            }
            return true;
        }

    }

    return false;
}

void Snake::Move()
{
    if(m_currentDirection==SnakeDirection::None)
        return;

    for(size_t it=m_snakeBody.size() - 1; it!=0; --it)
        m_snakeBody.at(it)=m_snakeBody.at(it - 1);

    switch(m_currentDirection)
    {
        case SnakeDirection::None: return;
        case SnakeDirection::Up: m_directionVec={0,-1};
            break;
        case SnakeDirection::Down: m_directionVec={0,1};
            break;
        case SnakeDirection::Left: m_directionVec={-1,0};
            break;
        case SnakeDirection::Right: m_directionVec={1,0};
            break;
    }
    m_snakeBody.front().m_position+=m_directionVec;

    if(m_currentDirection!=m_previewDirection)
    {
        if(m_previewDirection==SnakeDirection::Right)
        {
            if(m_currentDirection==SnakeDirection::Up)
            {
                m_snakeBody.front().m_rotation=180.f;
                m_snakeBody.at(1).m_body_part=SnakeBodyPart::tree_RightToUp;
            }
            else if(m_currentDirection==SnakeDirection::Down)
            {
                m_snakeBody.front().m_rotation=0.f;
                m_snakeBody.at(1).m_body_part=SnakeBodyPart::tree_RightToDown;
            }
        }
        else if(m_previewDirection==SnakeDirection::Left)
        {
            if(m_currentDirection==SnakeDirection::Up)
            {
                m_snakeBody.front().m_rotation=180.f;
                m_snakeBody.at(1).m_body_part=SnakeBodyPart::tree_LeftToUP;
            }
            else if(m_currentDirection==SnakeDirection::Down)
            {
                m_snakeBody.front().m_rotation=0.f;
                m_snakeBody.at(1).m_body_part=SnakeBodyPart::tree_LeftToDown;
            }
        }
        else if(m_previewDirection==SnakeDirection::Up)
        {
            if(m_currentDirection==SnakeDirection::Right)
            {
                m_snakeBody.front().m_rotation=-90.f;
                m_snakeBody.at(1).m_body_part=SnakeBodyPart::tree_UPToRight;
            }
            else if(m_currentDirection==SnakeDirection::Left)
            {
                m_snakeBody.front().m_rotation=90.f;
                m_snakeBody.at(1).m_body_part=SnakeBodyPart::tree_UpToLeft;
            }
        }
        else if(m_previewDirection==SnakeDirection::Down)
        {
            if(m_currentDirection==SnakeDirection::Right)
            {
                m_snakeBody.front().m_rotation=-90.f;
                m_snakeBody.at(1).m_body_part=SnakeBodyPart::tree_DownToRight;
            }
            else if(m_currentDirection==SnakeDirection::Left)
            {
                m_snakeBody.front().m_rotation=90.f;
                m_snakeBody.at(1).m_body_part=SnakeBodyPart::tree_DownToLeft;
            }
        }
        else if(m_previewDirection==SnakeDirection::None){}
    }
    else
    {
        m_snakeBody.at(1).m_body_part=SnakeBodyPart::tree;

        if(m_currentDirection==SnakeDirection::Left || m_currentDirection==SnakeDirection::Right)
            m_snakeBody.at(1).m_rotation=90.f;
        else
            m_snakeBody.at(1).m_rotation=0.f;
    }

    m_previewDirection=m_currentDirection;
}

SnakeDirection Snake::getDirection() const
{
    return m_currentDirection;
}

void Snake::setDirection(SnakeDirection l_direction)
{
    Snake::m_currentDirection=l_direction;
}

void Snake::reset(const sf::Vector2f& startPoint,SnakeDirection direction)
{
    assert(direction!=SnakeDirection::None && "direction can't be None");

    m_snakeBody.clear();
    switch(direction)
    {

        case SnakeDirection::Up:
        {
            m_snakeBody.push_back(SnakeSegment(startPoint.x,startPoint.y,SnakeBodyPart::Head));
            m_snakeBody.push_back(SnakeSegment(startPoint.x,startPoint.y + 1,SnakeBodyPart::tree));
            m_snakeBody.push_back(SnakeSegment(startPoint.x,startPoint.y + 2,SnakeBodyPart::Tail));
            m_snakeBody.front().m_rotation=180;
        }
            break;
        case SnakeDirection::Down:
        {
            m_snakeBody.push_back(SnakeSegment(startPoint.x,startPoint.y,SnakeBodyPart::Head));
            m_snakeBody.push_back(SnakeSegment(startPoint.x,startPoint.y - 1,SnakeBodyPart::tree));
            m_snakeBody.push_back(SnakeSegment(startPoint.x,startPoint.y - 2,SnakeBodyPart::Tail));
            m_snakeBody.front().m_rotation=0;
        }
            break;
        case SnakeDirection::Left:
        {
            m_snakeBody.push_back(SnakeSegment(startPoint.x,startPoint.y,SnakeBodyPart::Head));
            m_snakeBody.push_back(SnakeSegment(startPoint.x + 1,startPoint.y,SnakeBodyPart::tree));
            m_snakeBody.push_back(SnakeSegment(startPoint.x + 2,startPoint.y,SnakeBodyPart::Tail));
            m_snakeBody.front().m_rotation=90;        }
            break;
        case SnakeDirection::Right:
        {
            m_snakeBody.push_back(SnakeSegment(startPoint.x,startPoint.y,SnakeBodyPart::Head));
            m_snakeBody.push_back(SnakeSegment(startPoint.x - 1,startPoint.y,SnakeBodyPart::tree));
            m_snakeBody.push_back(SnakeSegment(startPoint.x - 2,startPoint.y,SnakeBodyPart::Tail));
            m_snakeBody.front().m_rotation=-90;
        }
            break;
        case SnakeDirection::None:
            break;
    }

    m_speed=2;
    m_directionVec={0,0};

    m_currentDirection=direction;
    m_previewDirection=m_currentDirection;

    m_currentHeadFrame=snakeBodyPartToRect(SnakeBodyPart::Head);
}

float Snake::getSpeed() const
{
    return m_speed;
}

void Snake::updateSpeed(const float speed)
{
    m_speed=speed;

    if(speed<m_speedMin)
        m_speed=m_speedMin;

    if(speed>m_speedMax)
        m_speed=m_speedMax;

}

bool Snake::isCollide() const
{
    return m_collide;
}

sf::Vector2f Snake::getPosition() const
{
    return m_snakeBody.front().m_position*m_sizeBlock + sf::Vector2f(m_sizeBlock/2.f,m_sizeBlock/2.f);
}

void Snake::render(Window& window)
{
    if(m_snakeBody.empty())
        return;

    for(auto it=m_snakeBody.begin() + 1; it!=m_snakeBody.end(); ++it)
    {
        m_bodySprite.setTextureRect(snakeBodyPartToRect(it->m_body_part));
        m_bodySprite.setPosition(it->m_position*m_sizeBlock + sf::Vector2f(m_sizeBlock/2.f,m_sizeBlock/2.f));

        if(it->m_body_part==SnakeBodyPart::tree)
            m_bodySprite.setRotation(it->m_rotation);
        else if(it->m_body_part==SnakeBodyPart::Tail)
        {
            switch(detectDirection(*(it-1),*it))
            {

                case SnakeDirection::Up: it->m_rotation=0.f;
                    break;
                case SnakeDirection::Down: it->m_rotation=-180.f;
                    break;
                case SnakeDirection::Left: it->m_rotation=-90.f;
                    break;
                case SnakeDirection::Right:it->m_rotation=90.f;
                    break;
                case SnakeDirection::None:
                    break;
            }
            m_bodySprite.setRotation(it->m_rotation);
        }

        window.draw(m_bodySprite);

        m_bodySprite.setRotation(0.f);

    }

    m_bodySprite.setTextureRect(m_currentHeadFrame);
    m_bodySprite.setRotation(m_snakeBody.front().m_rotation);
    m_bodySprite.setPosition(m_snakeBody.front().m_position*m_sizeBlock + sf::Vector2f(m_sizeBlock/2.f,m_sizeBlock/2.f));

    window.draw(m_bodySprite);

    m_bodySprite.setRotation(0.f);
}

void Snake::onHandleEvent(thor::ActionContext event)
{
    if(event.actionId==ID::getEventId(SnakeDirection::Up) and m_currentDirection!=SnakeDirection::Down)
    {
        m_currentDirection=SnakeDirection::Up;

    }else if(event.actionId==ID::getEventId(SnakeDirection::Down) and m_currentDirection!=SnakeDirection::Up)
    {
        m_currentDirection=SnakeDirection::Down;

    }else if(event.actionId==ID::getEventId(SnakeDirection::Right) and m_currentDirection!=SnakeDirection::Left)
    {
        m_currentDirection=SnakeDirection::Right;

    }else if(event.actionId==ID::getEventId(SnakeDirection::Left) and m_currentDirection!=SnakeDirection::Right)
    {
        m_currentDirection=SnakeDirection::Left;
    }
}

sf::IntRect Snake::snakeBodyPartToRect(SnakeBodyPart bodyPart)
{
    switch(bodyPart)
    {

        case SnakeBodyPart::Head:
            return {0,0,42,42};
            break;
        case SnakeBodyPart::tree:
            return {84,84,42,42};
            break;
        case SnakeBodyPart::tree_DownToLeft:
        case SnakeBodyPart::tree_RightToUp:
            return {84,42,42,42};
            break;
        case SnakeBodyPart::tree_RightToDown:
        case SnakeBodyPart::tree_UpToLeft:
            return {84,0,42,42};
            break;
        case SnakeBodyPart::tree_UPToRight:
        case SnakeBodyPart::tree_LeftToDown:
            return {42,0,42,42};
            break;
        case SnakeBodyPart::tree_LeftToUP:
        case SnakeBodyPart::tree_DownToRight:
            return {42,42,42,42};
            break;
        case SnakeBodyPart::Tail:
            return {42,84,42,42};
            break;
    }
}

SnakeDirection Snake::detectDirection(const Snake::SnakeSegment &preview,const Snake::SnakeSegment &last)
{

    if(preview.m_position.x==last.m_position.x and preview.m_position.y==last.m_position.y)
        return SnakeDirection::None;

    if(preview.m_position.x==last.m_position.x)
    {
        if(preview.m_position.y<last.m_position.y)
            return SnakeDirection::Up;
        else
            return SnakeDirection::Down;
    }
    else
    {
        if(preview.m_position.x<last.m_position.x)
            return SnakeDirection::Left;
        else
            return SnakeDirection::Right;
    }
}

void Snake::setSpeed(const float speed)
{
    m_speed=speed;

    if(speed<0.f)
        m_speed=0.f;
}
