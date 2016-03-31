#ifndef STAR_H
#define STAR_H
#include <glm/glm.hpp>
#include "object.h"

using glm::vec2;

class Star: public Object
{
    public:
        Star(vec2 center, vec2 vel, int index, double weight, double r);

        double radius;
};
#endif
