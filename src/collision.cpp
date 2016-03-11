#include <glm/glm.hpp>
#include "collision.h"
#include <algorithm>
#include <iostream>

using std::max;
using std::min;
using std::cout;
using std::endl;

int CollisionDetection::twoShapes(Object first, Object second)
{
}

int CollisionDetection::shapeAndPoint(Object object, vec2 point)
{
    for(unsigned int k = 0; k < object.collisionShapes.size(); k++)
    {
        int count = 0;
        for(unsigned int a = 0; a < object.collisionShapes.at(k).points.size() - 1; a++)
        {
            if(rayIntersects(point, object.collisionShapes.at(k).points.at(a) + object.position,
                                    object.collisionShapes.at(k).points.at(a + 1) + object.position))
            {
                count++;
            }
        }

        if(count % 2 == 1)
        {
            return (int)k;
        }
    }

    return -1;
}

bool CollisionDetection::rayIntersects(vec2 point, vec2 start, vec2 end)
{
    vec2 top;
    vec2 bottom;
    if(start.y > end.y)
    {
        top = start;
        bottom = end;
    }
    else
    {
        top = end;
        bottom = start;
    }

    if(point.y == top.y || point.y == bottom.y)
    {
        point.y += .0001;
    }

    if(point.y < bottom.y || point.y > top.y)
    {
        return false;
    }
    else if(point.x > max(top.x, bottom.x))
    {
        return false;
    }
    else
    {
        if(point.x < min(top.x, bottom.x))
        {
            return true;
        }
        else
        {
            float i = 999999999;
            float j = 999999999;

            if(abs(top.x - bottom.x) > .00001)
            {
                i = (top.y - bottom.y) / (top.x - bottom.x);
            }
            if(abs(bottom.x - point.x) > .00001)
            {
                j = (point.y - bottom.y) / (point.x - bottom.x);
            }

            if(j > i)
            {
                return true;
            }
        }
    }

    return false;
}
