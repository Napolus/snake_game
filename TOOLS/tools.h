//
// Created by napolus on 7/12/20.
//

#ifndef ALL____TOOLS_H
#define ALL____TOOLS_H


#include <random>
#include <cassert>
#include <type_traits>
#include <SFML/Graphics/Shape.hpp>
#include <Vectors/VectorAlgebra2D.hpp>

#include "Vectors.hpp"
namespace tools
{
    template<typename T>
    inline T Randomize(T min,T max)
    {
        assert( std::is_arithmetic_v<T> and min<=max and "template has need to be integer or min<=max");

        std::random_device random_device;
        std::mt19937_64 random_engine(random_device());
        std::uniform_int_distribution<T> distribution(min, max);

        return distribution(random_engine);
    }


    inline bool contain(const sf::Shape& polygon,sf::Vector2f& point)
    {
        if(!polygon.getGlobalBounds().contains(point))
            return false;

        const auto transformOfShape=polygon.getTransform();
        const float infinity=std::numeric_limits<float>::max();
        sf::Vector2f infinitePoint{point.x,infinity};

        const size_t nbPoint=polygon.getPointCount();

        sf::Vector2f pointA;
        sf::Vector2f pointB;

        size_t nbOfIntersection{0};
        for(size_t i=0;i<nbPoint;++i)
        {
             pointA= transformOfShape.transformPoint(polygon.getPoint(i));
            if (i==nbPoint-1)
               pointB=transformOfShape.transformPoint(polygon.getPoint(0));
            else
               pointB=transformOfShape.transformPoint(polygon.getPoint(i+1));

            sf::Vector2f intersectPoint;
            if(pointA==point or pointB==point)
                return true;
            if(thor::isIntersectedSegments(pointA,pointB,point,infinitePoint,intersectPoint))
            {
                if(intersectPoint==pointA or intersectPoint==pointB)
                    continue;
                if(intersectPoint==point)
                    return true;
                ++nbOfIntersection;
            }
        }

        return (nbOfIntersection%2)==0;

    }
}


#endif //SNAKE_GAME_TOOLS_H
