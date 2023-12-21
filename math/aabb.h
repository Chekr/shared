#ifndef AABB_H
#define AABB_H

#include "../../../shared/math/vec2.h"

typedef Vec2<int16_t> Vec2I16;

class Aabb
{
public:
    Aabb(Vec2f pos, Vec2f extents) :
        m_pos(pos),
        m_extents(extents)
    {}

    bool CircleCollision(
            const Vec2f& start, 
            const Vec2f& end, 
            const float& radius, 
            Vec2f& collision)
    {
        Vec2f nearestPoint;
        nearestPoint.x = std::max(m_pos.x, std::min(end.x, m_extents.x));
        nearestPoint.y = std::max(m_pos.y, std::min(end.y, m_extents.y));

        Vec2f rayToNearest = nearestPoint - end;
        float overlap = radius - rayToNearest.length();
        if(std::isnan(overlap)) { overlap = 0.0f; }
        if(overlap>0.0f)
        {
            collision = end-rayToNearest.normalize()*overlap;
            return true;
        }

        return false;
    }

public:
    Vec2f m_pos;
    Vec2f m_extents;
};


class AabbI16
{
public:
    AabbI16(Vec2I16 pos, Vec2I16 extents) :
        m_pos(pos),
        m_extents(extents)
    {}

    bool CircleCollision(
            const Vec2I16& start, 
            const Vec2I16& end, 
            const float& radius, 
            Vec2I16& collision)
    {
        Vec2I16 nearestPoint;
        nearestPoint.x = std::max(m_pos.x, std::min(end.x, m_extents.x));
        nearestPoint.y = std::max(m_pos.y, std::min(end.y, m_extents.y));

        Vec2I16 rayToNearest = nearestPoint - end;
        float overlap = radius - rayToNearest.length();
        if(std::isnan(overlap)) { overlap = 0; }
        if(overlap>0)
        {
            collision = end-rayToNearest.normalize()*overlap;
            return true;
        }

        return false;
    }

public:
    Vec2I16 m_pos;
    Vec2I16 m_extents;
};


#endif
