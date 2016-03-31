#ifndef ANIMATION_H
#define ANIMATION_H
#include <glm/glm.hpp>

using glm::vec2;

class Animation
{
    public:
        Animation(vec2 s, vec2 e, vec2 pos, vec2 vel, double ang, double rate, double a):
        start(s), end(e), position(pos), velocity(vel), angle(ang), turnRate(rate), age(a) {}
        vec2 start;
        vec2 end;
        vec2 position;
        vec2 velocity;
        double angle;
        double turnRate;
        double age;
};

#endif
