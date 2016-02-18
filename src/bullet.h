#ifndef BULLET_H
#define BULLET_H
#include <glm/glm.hpp>

using glm::vec2;

class Bullet
{
    public:
        vec2 *position;
        vec2 *velocity;
        double age;
};
#endif
