#ifndef ANIMATION_H
#define ANIMATION_H
#include <glm/glm.hpp>

using glm::vec2;

class Animation
{
    public:
        vec2 start;
        vec2 end;
        vec2 position;
        vec2 velocity;
        double angle;
        double turnRate;
        double age;
};

#endif
