#ifndef ANIMATION_H
#define ANIMATION_H
#include <glm/glm.hpp>
#include "object.h"

using glm::vec2;

class Animation: public Object
{
    public:
        Animation(vec2 center, vec2 vel, vec2 start, vec2 end, double age);
        double turnRate;
};

#endif
