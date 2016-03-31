#include "bullet.h"
#include "convex_shape.h"

Bullet::Bullet(vec2 center, vec2 vel, int index, double age):
Object(center, vel, index)
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
