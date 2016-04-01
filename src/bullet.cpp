#include "bullet.h"
#include "convex_shape.h"
#include <iostream>

using std::cout;
using std::endl;

Bullet::Bullet(vec2 center, vec2 vel, int index, double age):
Object(center, vel, index, 2)
{
    drawPoints = true;
    timeToLive = age;
    ConvexShape shape;
    shape.points.push_back(vec2(0,0));
    shape.points.push_back(vec2(0,0));
    shape.points.push_back(vec2(0,0));
    collisionShapes.push_back(shape);
    points.push_back(vec2(0,0));
    points.push_back(vec2(0,0));
}
