#ifndef ASTEROID_H
#define ASTEROID_H
#include <glm/glm.hpp>
#include <vector>
#include "object.h"

using glm::vec2;
using std::vector;

class Asteroid: public Object
{
    public:
        double radius;
};
#endif
