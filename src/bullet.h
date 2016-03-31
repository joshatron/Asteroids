#ifndef BULLET_H
#define BULLET_H
#include <glm/glm.hpp>
#include "object.h"

using glm::vec2;

class Bullet: public Object
{
    public:
        Bullet(vec2 center, vec2 vel, int index, double age);
};
#endif
