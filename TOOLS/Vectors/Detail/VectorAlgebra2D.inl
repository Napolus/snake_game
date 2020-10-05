/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2015 Jan Haller
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////

namespace thor
{
    template <typename T>
    sf::Vector2<T> vector(sf::Vector2<T> const& pointA, sf::Vector2<T> const& pointB)
    {
        return pointB-pointA;
    }

    template <typename T>
    T Distance(sf::Vector2<T> const& pointA, sf::Vector2<T> const& pointB)
    {
        return length(vector(pointA,pointB));
    }

    template <typename T>
    T length(const sf::Vector2<T>& vector)
    {
        return TrigonometricTraits<T>::sqrt(squaredLength(vector));
    }

    template <typename T>
    T squaredLength(const sf::Vector2<T>& vector)
    {
        return dotProduct(vector, vector);
    }

    template <typename T>
    void setLength(sf::Vector2<T>& vector, T newLength)
    {
        assert(vector != sf::Vector2<T>());
        vector *= newLength / length(vector);
    }

    template <typename T>
    sf::Vector2<T> unitVector(const sf::Vector2<T>& vector)
    {
        assert(vector != sf::Vector2<T>());
        return vector / length(vector);
    }

    template <typename T>
    T polarAngle(const sf::Vector2<T>& vector)
    {
        assert(vector != sf::Vector2<T>());
        return TrigonometricTraits<T>::arcTan2(vector.y, vector.x);
    }

    template <typename T>
    void setPolarAngle(sf::Vector2<T>& vector, T newAngle)
    {
        // No assert here, because turning a zero vector is well-defined (yields always zero vector)

        T vecLength = length(vector);

        vector.x = vecLength * TrigonometricTraits<T>::cos(newAngle);
        vector.y = vecLength * TrigonometricTraits<T>::sin(newAngle);
    }

    template <typename T>
    void rotate(sf::Vector2<T>& vector, T angle)
    {
        // No assert here, because turning a zero vector is well-defined (yields always zero vector)

        T cos = TrigonometricTraits<T>::cos(angle);
        T sin = TrigonometricTraits<T>::sin(angle);

        // Don't manipulate x and y separately, otherwise they're overwritten too early
        vector = sf::Vector2<T>(
            cos * vector.x - sin * vector.y,
            sin * vector.x + cos * vector.y);
    }

    template <typename T>
    sf::Vector2<T> rotatedVector(const sf::Vector2<T>& vector, T angle)
    {
        // No assert here, because turning a zero vector is well-defined (yields always zero vector)

        sf::Vector2<T> copy = vector;
        rotate(copy, angle);
        return copy;
    }

    template <typename T>
    sf::Vector2<T> perpendicularVector(const sf::Vector2<T>& vector)
    {
        return sf::Vector2<T>(-vector.y, vector.x);
    }

    // ---------------------------------------------------------------------------------------------------------------------------


    template <typename T>
    T signedAngle(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs)
    {
        assert(lhs != sf::Vector2<T>() && rhs != sf::Vector2<T>());
        return TrigonometricTraits<T>::arcTan2(crossProduct(lhs, rhs), dotProduct(lhs, rhs));
    }

    template <typename T>
    T dotProduct(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs)
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    template <typename T>
    T crossProduct(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs)
    {
        return lhs.x * rhs.y - lhs.y * rhs.x;
    }

    template <typename T>
    sf::Vector2<T> cwiseProduct(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs)
    {
        return sf::Vector2<T>(lhs.x * rhs.x, lhs.y * rhs.y);
    }

    template <typename T>
    sf::Vector2<T> cwiseQuotient(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs)
    {
        assert(rhs.x != 0 && rhs.y != 0);
        return sf::Vector2<T>(lhs.x / rhs.x, lhs.y / rhs.y);
    }

    template <typename T>
    sf::Vector2<T> projectedVector(const sf::Vector2<T>& vector, const sf::Vector2<T>& axis)
    {
        assert(axis != sf::Vector2<T>());
        return dotProduct(vector, axis) / squaredLength(axis) * axis;
    }

    template <typename T>
    bool isAligned(const sf::Vector2<T>& pointA,const sf::Vector2<T>& pointB,const sf::Vector2<T>& pointC)
    {
        const auto determinant=thor::crossProduct(thor::vector(pointA,pointB),thor::vector(pointA,pointC));

        return determinant==static_cast<T>(0);
    }

    template<typename  T>
    bool  segmentContainPoint(const sf::Vector2<T>& segA,const sf::Vector2<T>& segB,const sf::Vector2<T>& pointC)
    {
        if(thor::isAligned(segA,segB,pointC))
        {
            const auto &scalar=thor::dotProduct(thor::vector(segA,segB),thor::vector(segA,pointC));
            if(static_cast<T>(0)<=scalar && scalar<=thor::squaredLength(thor::vector(segA,segB)))
                return true;

            return false;
        }
        return false;
    }

    template <typename T>
    bool  isIntersectedSegments(const sf::Vector2<T>& seg1A,
                                const sf::Vector2<T>& seg1B,
                                const sf::Vector2<T>& seg2C,
                                const sf::Vector2<T>& seg2D,
                                sf::Vector2<T>& intersection)
    {
        const auto dr1=thor::vector(seg1A,seg1B);
        const auto dr2=thor::vector(seg2C,seg2D);

        const auto determinant=thor::crossProduct(dr1,dr2);
        if(determinant==static_cast<T>(0))
        {
            if(thor::segmentContainPoint(seg1A,seg1B,seg2C))
            {
                intersection=seg2C;
                return true;
            }
            if(thor::segmentContainPoint(seg1A,seg1B,seg2D))
            {
                intersection=seg2D;
                return true;
            }
            if(thor::segmentContainPoint(seg2C,seg2D,seg1A))
            {
                intersection=seg1A;
                return true;
            }
            if(thor::segmentContainPoint(seg2C,seg2D,seg1B))
            {
                intersection=seg1B;
                return true;
            }
            return false;
        }

        const float paramDr1=(thor::crossProduct(seg1A,dr2) +thor::crossProduct(seg2C,dr2))/-static_cast<float>(determinant);
        if(paramDr1<0 || paramDr1>1)
            return false;

        const float paramDr2=(thor::crossProduct(seg1A,dr1) +thor::crossProduct(seg2C,dr1))/-static_cast<float>(determinant);
        if(paramDr2<0 || paramDr2>1)
            return false;

        intersection = seg1A+(dr1*paramDr1);
        return true;
    }

} // namespace thor
