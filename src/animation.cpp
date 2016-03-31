#include "animation.h"

Animation::Animation(vec2 center, vec2 vel, vec2 start, vec2 end, double age):
Object(center, vel, -1)
{
    points.push_back(start);
    points.push_back(end);
    timeToLive = age;
}
