#include <glm/glm.hpp>
#include "collision.h"
#include <algorithm>
#include <iostream>

using std::max;
using std::min;
using glm::normalize;
using glm::dot;
using std::cout;
using std::endl;

int CollisionDetection::twoShapes(Object first, Object second)
{
    for(unsigned int k = 0; k < first.collisionShapes.size(); k++)
    {
        for(unsigned int a = 0; a < second.collisionShapes.size(); a++)
        {
            //find axes
            vector<vec2> normals;
            int size = first.collisionShapes.at(k).points.size();
            for(int t = 0; t < size; t++)
            {
                vec2 start;
                start.x = first.collisionShapes.at(k).points.at(t).x;
                start.y = first.collisionShapes.at(k).points.at(t).y;
                transform(start, first.position, first.angle);

                vec2 end;
                end.x = first.collisionShapes.at(k).points.at((t + 1) % size).x;
                end.y = first.collisionShapes.at(k).points.at((t + 1) % size).y;
                transform(end, first.position, first.angle);

                double dx = end.x - start.x;
                double dy = end.y - start.y;
                normals.push_back(vec2(-1 * dy, dx));
            }

            size = second.collisionShapes.at(a).points.size();
            for(int t = 0; t < size; t++)
            {
                vec2 start;
                start.x = second.collisionShapes.at(a).points.at(t).x;
                start.y = second.collisionShapes.at(a).points.at(t).y;
                transform(start, second.position, second.angle);

                vec2 end;
                end.x = second.collisionShapes.at(a).points.at((t + 1) % size).x;
                end.y = second.collisionShapes.at(a).points.at((t + 1) % size).y;
                transform(end, second.position, second.angle);

                double dx = end.x - start.x;
                double dy = end.y - start.y;
                normals.push_back(vec2(-1 * dy, dx));
            }
            
            bool all = true;
            size = normals.size();
            for(int t = 0; t < size; t++)
            {
                double minFirst = dot(normals.at(t), first.collisionShapes.at(k).points.at(0) + first.position);
                double maxFirst = minFirst;
                for(unsigned int i = 0; i < first.collisionShapes.at(k).points.size(); i++)
                {
                    double p = dot(normals.at(t), first.collisionShapes.at(k).points.at(i) + first.position);
                    if(p < minFirst)
                    {
                        minFirst = p;
                    }
                    else if(p > maxFirst)
                    {
                        maxFirst = p;
                    }
                }

                double minSecond = dot(normals.at(t), second.collisionShapes.at(a).points.at(0) + second.position);
                double maxSecond = minSecond;
                for(unsigned int i = 0; i < second.collisionShapes.at(a).points.size(); i++)
                {
                    double p = dot(normals.at(t), second.collisionShapes.at(a).points.at(i) + second.position);
                    if(p < minSecond)
                    {
                        minSecond = p;
                    }
                    else if(p > maxSecond)
                    {
                        maxSecond = p;
                    }
                }

                if(maxFirst < minSecond || maxSecond < minFirst)
                {
                    all = false;
                    break;
                }
            }

            if(all)
            {
                return k;
            }
        }
    }

    return -1;
}

int CollisionDetection::shapeAndPoint(Object object, vec2 point)
{
    for(unsigned int k = 0; k < object.collisionShapes.size(); k++)
    {
        int count = 0;
        int size = object.collisionShapes.at(k).points.size();
        for(int a = 0; a < size; a++)
        {
            vec2 start;
            start.x = object.collisionShapes.at(k).points.at(a).x;
            start.y = object.collisionShapes.at(k).points.at(a).y;
            transform(start, object.position, object.angle);

            vec2 end;
            end.x = object.collisionShapes.at(k).points.at((a + 1) % size).x;
            end.y = object.collisionShapes.at(k).points.at((a + 1) % size).y;
            transform(end, object.position, object.angle);

            if(rayIntersects(point, start, end))
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

void CollisionDetection::transform(vec2& point, vec2 transform, double angle)
{
    point.x = point.x * cos(angle) - point.y * sin(angle);
    point.y = point.x * sin(angle) + point.y * cos(angle);
    point += transform;
}
