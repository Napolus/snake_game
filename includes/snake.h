//
// Created by napolus on 7/10/20.
//

#ifndef SNAKE_GAME_SNAKE_H
#define SNAKE_GAME_SNAKE_H


#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>


#include <vector>


#include "ManageAction/EventManager.hpp"
#include "enumerations.hpp"

class Window;


class Snake : public Listener
{
    struct SnakeSegment
    {
        SnakeSegment(float x,float y,SnakeBodyPart bodyPart):m_position(x,y),
                                                             m_body_part(bodyPart),
                                                             m_rotation(0.f){}
        SnakeSegment& operator=(const SnakeSegment& other)
        {
            m_position=other.m_position;

            if(m_body_part!=SnakeBodyPart::Tail)
                m_rotation=other.m_rotation;

            if(!(other.m_body_part==SnakeBodyPart::Head||other.m_body_part==SnakeBodyPart::Tail
                 ||m_body_part==SnakeBodyPart::Head||m_body_part==SnakeBodyPart::Tail))
                    m_body_part=other.m_body_part;
            return *this;
        }
        sf::Vector2f        m_position;
        SnakeBodyPart       m_body_part;
        float               m_rotation;
    };

    using SnakeBody=std::deque<SnakeSegment>;

    friend class GameState;
    friend class World;
public:

    Snake(float sizeByBlock,sf::Texture& texture,const sf::Vector2f& startPoint=sf::Vector2f(1,1));

    ~Snake() {std::cout<<"le serpent est detruit"<<std::endl;};

    void update(sf::Time deltatime);

    void grow();

    void render(Window &window);

    [[nodiscard]] SnakeDirection getDirection() const;

    float getSpeed() const;

    void updateSpeed(const float speed);

    void setSpeed(const float speed);

    void setDirection(SnakeDirection l_direction);

    bool isCollide() const;

    sf::Vector2f getPosition() const;

    void onHandleEvent(thor::ActionContext event) override;

    void reset(const sf::Vector2f& startPoint,SnakeDirection direction);
private:

    static sf::IntRect snakeBodyPartToRect(SnakeBodyPart bodyPart);

    void initSprite();

    bool checkCollision();

    void Move();

    static SnakeDirection detectDirection(const SnakeSegment& preview,const SnakeSegment& last);

private:
    //graphics
    sf::IntRect         m_currentHeadFrame;
    sf::Sprite          m_bodySprite;
    sf::Texture&        m_texture;
    SnakeBody           m_snakeBody;
    float               m_sizeBlock;

    //animation
    float               m_dtAnimationHead;

    //physics
    SnakeDirection      m_currentDirection;
    SnakeDirection      m_previewDirection;
    float               m_timeToUpdate;
    const float         m_speedMin=10.f;
    const float         m_speedMax=25.f;
    float               m_speed;
    bool                m_can_cut;
    bool                m_collide;
    sf::Vector2f        m_directionVec;
};


#endif //SNAKE_GAME_SNAKE_H
